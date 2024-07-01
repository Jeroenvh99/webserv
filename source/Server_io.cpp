#include "Server.hpp"
#include "http/Response.hpp"

Server::IOStatus
Server::_parse_request(Client& client) {
	webserv::Buffer	buf;

	if (_recv(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	try {
		if (client.parse_request(buf) == true) {
			_elog.log(LogLevel::debug, std::string(client.address()),
				": Request parsing finished.");
			client.respond({client, *this});
			return (IOStatus::success);
		}
	} catch (http::Parser::VersionException& e) {
		_elog.log(LogLevel::error, std::string(client.address()),
			": Parse error: ", e.what());
		client.respond({http::Status::version_not_supported, *this});
		return (IOStatus::failure);
	} catch (http::Parser::Exception& e) {
		_elog.log(LogLevel::error, std::string(client.address()),
			": Parse error: ", e.what());
		client.respond({http::Status::bad_request, *this});
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}

Server::IOStatus
Server::_parse_response(Client& client) {
	webserv::Buffer	buf;

	if (_fetch(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	try {
		if (client.parse_response(buf)) {
			_elog.log(LogLevel::debug, std::string(client.address()),
				": Response parsing from CGI finished.");
			return (IOStatus::success);
		}
	} catch (http::Parser::Exception& e) {
		_elog.log(LogLevel::error, std::string(client.address()),
			"CGI parsing error: ", e.what());
		client.respond({http::Status::internal_error, *this});
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}

Server::IOStatus
Server::_deliver(Client& client) {
	webserv::Buffer	buf;

	if (_recv(client, buf) == IOStatus::failure)
		return (IOStatus::failure);

	job::StatusOption	status = client.deliver(buf);

	if (status && http::is_error(*status)) {
		_elog.log(LogLevel::error, std::string(client.address()),
			": Error delivering resource: ", *status, ".");
		client.respond({*status, *this});
		return (IOStatus::failure);
	}
	_elog.log(LogLevel::debug, std::string(client.address()),
		": Delivered ", buf.len(), " bytes.");
	return (IOStatus::success);
}

Server::IOStatus
Server::_fetch_and_send(Client& client) {
	webserv::Buffer	buf;

	if (_fetch(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	return (_send(client, buf));
}

Server::IOStatus
Server::_fetch(Client& client, webserv::Buffer& buf) {
	job::StatusOption const	status = client.fetch(buf);

	if (status && http::is_error(*status)) {
		_elog.log(LogLevel::error, std::string(client.address()),
			": Error fetching resource:", *status, ".");
		client.respond({*status, *this});
		return (IOStatus::failure);
	}
	_elog.log(LogLevel::debug, std::string(client.address()),
		": Fetched ", buf.len(), " bytes.");
	return (IOStatus::success);
}

Server::IOStatus
Server::_recv(Client& client, webserv::Buffer& buf) {
	try {
		size_t const	bytes = client.socket().read(buf);

		if (bytes == 0)
			return (IOStatus::failure);
		_elog.log(LogLevel::debug, std::string(client.address()),
			": Received ", bytes, " bytes.");
	} catch (Client::Socket::Exception& e) {
		_elog.log(LogLevel::error, std::string(client.address()),
			": Networking failure: ", e.what());
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}

Server::IOStatus
Server::_send(Client& client, webserv::Buffer const& buf) {
	try {
		size_t const	bytes = client.socket().write(buf);

		if (bytes == 0)
			return (IOStatus::failure);
		_elog.log(LogLevel::debug, std::string(client.address()),
			": Sent ", bytes, " bytes.");
	} catch (Client::Socket::Exception& e) {
		_elog.log(LogLevel::error, e.what());
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}
