#include "Server.hpp"

bool
Server::_read(Client& client) {
	size_t const	bytes_read = client.recv();

	if (bytes_read == 0)
		return (false);
	_elog.log(LogLevel::debug, "Received ", bytes_read, " bytes.");
	try {
		client.parse();
	} catch (http::Request::Parser::Exception& e) {
		_elog.log(LogLevel::error, "Parse error: ", e.what());
		return (false);
	}
	if (client.state() == Client::State::wait)
		_elog.log(LogLevel::info, "Received request:\n", std::string(client.request()));
	return (true);
}

bool
Server::_write(Client& client) {
	size_t const	bytes_written = client.send();

	if (bytes_written == 0)
		return (false);
	_elog.log(LogLevel::debug, "Sent ", bytes_written, " bytes.");
	return (true);
}
