#include "Server.hpp"

void
Server::process(int poller_timeout) { // DB: this could be made configurable
	for (auto const& event: _poller.wait<128>(poller_timeout)) {
		SharedHandle	box = event.handle();

		if (box == _acceptor)
			_accept();
		else {
			ClientIt	it = _clients.find(box);

			if (it == _clients.end()) {
				it = _graveyard.find(box);
				_process_graveyard(event, it);
			} else
				_process_core(event, it);
		}
	}
}

void
Server::_process_core(Poller::Event const& event, ClientIt it) {
	Client	client(*it);

	switch (client.state()) {
	case Client::State::idle:
	case Client::State::parse:
		if (event.happened(Poller::EventType::read) && _read(client) == false)
			_to_graveyard(it);
		break;
	case Client::State::fetch:
		_fetch(client);
		break;
	case Client::State::wait:
		_wait(client);
		break;
	case Client::State::send:
		if (event.happened(Poller::EventType::write) && _send(client) == false)
			_to_graveyard(it);
		break;
	}
}

void
Server::_process_graveyard(Poller::Event const& event, ClientIt it) {
	Client	client(*it);

	if (client.state() == Client::State::send
		&& event.happened(Poller::EventType::write)
		&& _send(client) == true)
		return;
	_drop(it);
}

void
Server::_to_graveyard(ClientIt it) {
	_graveyard.insert(_clients.extract(it));
}
