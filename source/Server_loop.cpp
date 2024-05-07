#include "Server.hpp"

# include <algorithm>
# include <iostream>

void
Server::loop(int backlog_size) {
	((Acceptor&)(*_acceptor)).listen(backlog_size);
	while (true)
		_process_events(_poller.wait<128>(1000));
}

void
Server::_process_events(Poller::Events const& events) {
	for (auto const& event: events) {
		SharedHandle	box = event.handle();
		auto			f = [box](Client const& client) {
			return (client.socket_box() == box);
		};

		if (box == _acceptor)
			_add_client();
		else {
			Clients::iterator const	it = std::find_if(_clients.begin(), _clients.end(), f);

			_handle_client(event, it);
		}
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
	Client&				client = _clients.emplace_back(acceptor().accept(address)); // requires C++17

	_poller.add_shared(client.socket_box(), poller_events, poller_mode);
	std::cout << "Connection established at " << std::string(address) << std::endl; // replace with notice-level log message
}

void
Server::_drop_client(Clients::iterator client) {
	std::cout << "Connection lost." << std::endl; // replace with notice-level log message
	_poller.remove(client->socket_box());
	_clients.erase(client);
}

void
Server::_process_buffer(Client& client) {
	std::cout << "Received: " << _buffer; // replace with notice-level log message
	client.request().addBuffer(_buffer);
}
