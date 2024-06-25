#include "Server.hpp"
#include "http/Response.hpp"

Server::IOStatus
Server::_parse(Client& client) {
	webserv::Buffer	buf;

	if (_recv(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	try {
		if (client.parse(buf) == http::Request::Parser::State::done) {
			client.respond({client, *this}); // ignore job status for now
			return (IOStatus::success);
		}
	} catch (http::Request::Parser::VersionException& e) {
		_elog.log(LogLevel::error, "Parse error: ", e.what());
		client.respond({http::Status::version_not_supported, *this});
		return (IOStatus::failure);
	} catch (http::Request::Parser::Exception& e) {
		_elog.log(LogLevel::error, "Parse error: ", e.what());
		client.respond({http::Status::bad_request, *this});
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}

Server::IOStatus
Server::_deliver(Client& client) {
	webserv::Buffer	buf;

	if (_recv(client, buf) == IOStatus::failure)
		return (IOStatus::failure);

	job::StatusOption	job_status = client.deliver(buf);

	if (job_status && http::is_error(*job_status)) {
		client.respond({*job_status, *this});
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}

Server::IOStatus
Server::_fetch_headers(Client& client) {

}

Server::IOStatus
Server::_fetch_body(Client& client) {
	webserv::Buffer	buf;

	if (_flush(client, buf) == 0) {
		job::StatusOption	job_status = client.fetch_body(buf);

		if (job_status && http::is_error(*job_status)) {
			client.respond({*job_status, *this});
			return (IOStatus::failure);
		}
	}
	return (_send(client, buf));
}

Server::IOStatus
Server::_recv(Client& client, webserv::Buffer& buf) {
	try {
		size_t const	bytes = client.socket().read(buf);

		if (bytes == 0)
			return (IOStatus::failure);
		_elog.log(LogLevel::debug, "Received ", bytes, " bytes.");
	} catch (Client::Socket::Exception& e) {
		_elog.log(LogLevel::error, e.what());
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
		_elog.log(LogLevel::debug, "Sent ", bytes, " bytes.");
	} catch (Client::Socket::Exception& e) {
		_elog.log(LogLevel::error, e.what());
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}
