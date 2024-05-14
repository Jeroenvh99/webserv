#include "Server.hpp"

# include <iostream>

void
Server::loop(int backlog_size) {
	acceptor().listen(backlog_size);
	while (true) { // DB: there ought to be a cleaner way to shut down the server
		Poller::Events	events(_poller.wait<128>(1000));

		_process_events(events);
	}
}
// DB: make poller timeout and backlog size configurable?

void
Server::_process_events(Poller::Events const& events) {
	for (auto const& event: events) {
		SharedHandle	box = event.handle();

		if (box == _acceptor)
			_client_add();
		else {
			ClientMap::iterator	it = _clients.find(box);

			if (it == _clients.end()) {
				it = _graveyard.find(box);
				_client_handle_graveyard(it, event);
			} else
				_client_handle(it, event);
		}
	}
}

void
Server::_client_handle(ClientMap::iterator it, Poller::Event const& event) {
	Client	client(*it);

	_buffer.empty();
	if (event.happened(Poller::EventType::read)) {
		client.socket().read(_buffer);
		if (_buffer.len() == 0)
			_client_to_graveyard(it);
		else
			_process_buffer(client);
	} else if (event.happened(Poller::EventType::write))
		client.socket().write(_buffer);
}

void
Server::_client_handle_graveyard(ClientMap::iterator it, Poller::Event const& event) {
	Client	client(*it);

	if (client.state() == Client::State::send
		&& event.happened(Poller::EventType::write)) {
		_elog.log(LogLevel::info, "Something is to be written to the client now.");
	} else
		_client_drop(it);
}

void
Server::_client_add() {
	Acceptor::Address	address;
	Client::Socket		socket = acceptor().accept(address);

	_elog.log(LogLevel::notice,
		"Connection established at ", std::string(address));
	Client::SocketBox	box = _poller.add(std::move(socket), poller_events, poller_mode);
	_clients.insert(std::make_pair(box, ClientData()));
}

void
Server::_client_to_graveyard(ClientMap::iterator it) {
	_graveyard.insert(_clients.extract(it));
}

void
Server::_client_drop(ClientMap::iterator it) {
	_poller.remove(it->first);
	_graveyard.erase(it);
	_elog.log(LogLevel::notice, "Connection terminated.");
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
