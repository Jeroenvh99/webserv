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
				it = _graveyard.find(box); // either contained in core or graveyard
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
	case Client::State::parse_request:
		if (event.happened(Poller::EventType::read)
			&& _parse_request(client) == IOStatus::failure)
			_to_graveyard(it);
		break;
	case Client::State::parse_response:
		if (event.happened(Poller::EventType::read)
			&& _deliver(client) == IOStatus::failure)
			_to_graveyard(it);
		if (event.happened(Poller::EventType::write)
			&& _parse_response(client) == IOStatus::failure)
			_to_graveyard(it);
		break;
	case Client::State::work:
		if (event.happened(Poller::EventType::read)
			&& _deliver(client) == IOStatus::failure)
			_to_graveyard(it);
		if (event.happened(Poller::EventType::write)){
			_fetch_and_send(client);
			_to_graveyard(it);}
		break;
	}
}

void
Server::_process_graveyard(Poller::Event const& event, ClientIt it) {
	Client	client(*it);

	if (client.state() == Client::State::work
		&& event.happened(Poller::EventType::write)
		&& _fetch_and_send(client) == IOStatus::success)
		return;
	_drop(it);
}

void
Server::_to_graveyard(ClientIt it) {
	_graveyard.insert(_clients.extract(it));
}
