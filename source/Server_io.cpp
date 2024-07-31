#include "Server.hpp"
#include "http/Response.hpp"

Server::IOStatus
Server::_parse_request(Client& client) {
	webserv::Buffer	buf;

	if (_recv(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	_elog.log(LogLevel::debug, std::string(client.address()),
		": Directing ", buf.len(), " bytes to request parser.");
	try {
		if (client.parse_request(buf) == true) {
			client.respond({client, *this});
			_elog.log(LogLevel::debug, std::string(client.address()),
				": Request parsing finished; ", buf.len(), " trailing bytes.");
		}
	} catch (Client::RedirectionException& e) {
		_elog.log(LogLevel::error, std::string(client.address()),
			": Verkeerd verbonden: ", e.what());
		return (IOStatus::failure);
	} catch (Client::ErrorException& e) {
		_elog.log(LogLevel::error, std::string(client.address()),
			": An error happened: ", e.what());
		return (IOStatus::failure);
	} catch (http::parse::VersionException& e) {
		_elog.log(LogLevel::error, std::string(client.address()),
			": Parse error: ", e.what());
		client.respond({http::Status::version_not_supported, *this});
		return (IOStatus::failure);
	} catch (http::parse::Exception& e) {
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
	if (buf.len() == 0)
		return (IOStatus::success);
	_elog.log(LogLevel::debug, std::string(client.address()),
		": Directing ", buf.len(), " bytes to response parser.");
	try {
		if (client.parse_response(buf)) {
			_elog.log(LogLevel::debug, std::string(client.address()),
				": Response parsing from CGI finished.");
		}
	} catch (http::parse::Exception& e) {
		_elog.log(LogLevel::error, std::string(client.address()),
			": CGI parsing error: ", e.what());
		client.respond({http::Status::internal_error, *this});
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}

Server::IOStatus
Server::_dechunk(Client&) {
	return (IOStatus::failure); // placeholder
}

Server::IOStatus
Server::_deliver(Client& client) {
	webserv::Buffer	buf;

	if (_recv(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	switch (client.deliver(buf)) {
	case Client::OperationStatus::success:
	case Client::OperationStatus::pending:
		_elog.log(LogLevel::debug, std::string(client.address()),
			": Delivered ", buf.len(), " bytes.");
		return (IOStatus::success);
	case Client::OperationStatus::failure:
		_elog.log(LogLevel::error, std::string(client.address()),
			": Error delivering resource.");
		// todo: inject error message into body
		return (IOStatus::failure);
	default: // timeout
		__builtin_unreachable();
	}
}

Server::IOStatus
Server::_fetch_and_send(Client& client) {
	webserv::Buffer	buf;

	if (_fetch(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	return (_send(client, buf));
}

Server::IOStatus
Server::_enchunk_and_send(Client& client) {
	return (_fetch_and_send(client)); // placeholder
}

Server::IOStatus
Server::_fetch(Client& client, webserv::Buffer& buf) {
	switch (client.fetch(buf)) {
	case Client::OperationStatus::success:
		_elog.log(LogLevel::debug, std::string(client.address()),
			": Resource fetched successfully.");
		return (Server::IOStatus::success);
	case Client::OperationStatus::pending:
		if (buf.len() > 0)
			_elog.log(LogLevel::debug, std::string(client.address()),
				": Fetched ", buf.len(), " bytes.");
		return (Server::IOStatus::success);
	case Client::OperationStatus::timeout:
		_elog.log(LogLevel::error, std::string(client.address()),
			": Timeout occurred whilst fetching resource.");
		return (IOStatus::failure);
	case Client::OperationStatus::failure:
		_elog.log(LogLevel::error, std::string(client.address()),
			": Error fetching resource.");
		// inject error message into body?
		return (IOStatus::failure);
	default:
		__builtin_unreachable();
	}
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
	if (buf.len() == 0)
		return (IOStatus::success);
	try {
		size_t const	bytes = client.socket().write(buf);

		if (bytes != buf.len())
			return (IOStatus::failure);
		_elog.log(LogLevel::debug, std::string(client.address()),
			": Sent ", bytes, " bytes.");
	} catch (Client::Socket::Exception& e) {
		_elog.log(LogLevel::error, e.what());
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}
