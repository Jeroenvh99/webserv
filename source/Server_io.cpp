#include "Server.hpp"
#include "http/Response.hpp"

bool
Server::_read(Client& client) {
	try {
		size_t const	bytes = client.recv();

		if (bytes == 0)
			return (false);
		_elog.log(LogLevel::debug, "Received ", bytes, " bytes.");
	} catch (Client::Socket::Exception& e) {
		_elog.log(LogLevel::error, e.what());
		return (false);
	}
	try {
		client.parse();
	} catch (http::Request::Parser::VersionException& e) {
		_elog.log(LogLevel::error, "Parse error: ", e.what());
		client << respond_error(http::Status::version_not_supported);
		return (false);
	} catch (http::Request::Parser::Exception& e) {
		_elog.log(LogLevel::error, "Parse error: ", e.what());
		client << respond_error(http::Status::bad_request);
		return (false);
	}
	if (client.state() == Client::State::fetch)
		_elog.log(LogLevel::info, "Received request:\n", std::string(client.request()));
	return (true);
}

bool
Server::_fetch(Client& client) {
	route::Location const	loc = locate(client.request().uri().path());

	if (loc.is_cgi())
		client.cgi().launch(loc, client, _env);
	else
		client << respond(client.request());
	return (true);
}

bool
Server::_wait(Client& client) {
	client.wait();
	return (true);
}

bool
Server::_send(Client& client) {
	try {
		size_t const	bytes = client.send();

		if (bytes == 0)
			return (false);
		_elog.log(LogLevel::debug, "Sent ", bytes, " bytes.");
	} catch (Client::Socket::Exception& e) {
		_elog.log(LogLevel::error, e.what());
		return (false);
	}
	return (true);
}
