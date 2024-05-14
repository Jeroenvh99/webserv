#include "Server.hpp"

# include <algorithm>
# include <iostream>

void
Server::loop(int backlog_size) {
	acceptor().listen(backlog_size);
	while (true) // DB: there ought to be a cleaner way to shut down the server
		_process_events(_poller.wait<128>(1000));
}
// DB: make poller timeout and backlog size configurable?

void
Server::_process_events(Poller::Events const& events) {
	for (auto const& event: events) {
		SharedHandle	box = event.handle();
		auto			test = [box](Client const& client) {
			return (client.socket_box() == box);
		};

		if (box == _acceptor)
			_add_client();
		else
			_handle_client(event,
				std::find_if(_clients.begin(), _clients.end(), test));
	}
}

void
Server::_handle_client(Poller::Event const& event, Clients::iterator it) {
	_buffer.empty();
	if (event.happened(Poller::EventType::read)) {
		it->socket().read(_buffer);
		if (_buffer.len() == 0)
			_drop_client(it);
		else
			_process_buffer(*it);
	} else if (event.happened(Poller::EventType::write))
		it->socket().write(_buffer);
}

void
Server::_add_client() {
	Acceptor::Address	address;
	Client&				client = _clients.emplace_back(acceptor().accept(address));

	_poller.add_shared(client.socket_box(), poller_events, poller_mode);
	_elog.log(LogLevel::notice,
		"Connection established at ", std::string(address));
}

void
Server::_drop_client(Clients::iterator client) {
	_poller.remove(client->socket_box());
	_clients.erase(client);
	_elog.log(LogLevel::notice, "Connection lost.");
}

void
Server::_process_buffer(Client& client) {
	auto&	parser = client.parser();
	_elog.log(LogLevel::notice, "Received: ", _buffer);
	try {
		parser.parse(_buffer, client.request());
		if (parser.state() == http::Request::Parser::State::done)
			_elog.log(LogLevel::notice,
				"Parsed request:\n", std::string(client.request()));
	} catch (http::Request::Parser::Exception& e) {
		parser.clear(); // note 1
		_elog.log(LogLevel::error, "Error encountered whilst parsing buffer:", e.what());
		// send error response to client
	}
}
/* Notes:
 * 1	DB: Either make sure that all data that was part of the faulty request
 * 		is properly discarded (as to not interfere with a possible follow-up
 * 		request), or close this connection altogether. The latter option seems
 * 		the easiest.
 */
