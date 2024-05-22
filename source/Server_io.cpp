#include "Server.hpp"
#include "http/Response.hpp"

bool
Server::_read(Client& client) {
	size_t const	bytes = client.recv();

	if (bytes == 0)
		return (false);
	_elog.log(LogLevel::debug, "Received ", bytes, " bytes.");
	try {
		client.parse();
	} catch (http::Request::Parser::Exception& e) {
		_elog.log(LogLevel::error, "Parse error: ", e.what());
		client << http::Response(e.what(), http::StatusCode::bad_request); // DB: replace with access error page
		return (false);
	}
	if (client.state() == Client::State::wait)
		_elog.log(LogLevel::info, "Received request:\n", std::string(client.request()));
	return (true);
}

bool
Server::_fetch(Client& client) {
	client.fetch();
	return (true);
}

bool
Server::_wait(Client& client) {
	client.wait();
	return (true);
}

bool
Server::_send(Client& client) {
	size_t const	bytes = client.send();

	if (bytes == 0)
		return (false);
	_elog.log(LogLevel::debug, "Sent ", bytes, " bytes.");
	return (true);
}
