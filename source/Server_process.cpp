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
	Client		client(*it);

	if (event.happened(Poller::EventType::hangup)) {
		_elog.log(LogLevel::notice, std::string(client.address()),
			"Connection closed by peer.");
		_to_graveyard(it);
	}
	if (event.happened(Poller::EventType::read)
		&& _process_read(client) == IOStatus::failure)
		_to_graveyard(it);
	if (event.happened(Poller::EventType::write)
		&& _process_write(client) == IOStatus::failure)
		_to_graveyard(it);
}

Server::IOStatus
Server::_process_read(Client& client) {
	switch (client.istate()) {
	case Client::InputState::parse_request:
		return (_parse_request(client));
	case Client::InputState::deliver:
		return (_deliver(client));
	case Client::InputState::dechunk:
		return (_dechunk(client));
	default:	// closed
		return (IOStatus::failure); // ?
	}
}

Server::IOStatus
Server::_process_write(Client& client) {
	switch (client.ostate()) {
	case Client::OutputState::parse_response:
		return (_parse_response(client));
	case Client::OutputState::fetch:
		return (_fetch_and_send(client));
	case Client::OutputState::enchunk:
		return (_enchunk_and_send(client));
	default:	// closed
		return (IOStatus::success); // ?
	}
}

void
Server::_process_graveyard(Poller::Event const& event, ClientIt it) {
	Client	client(*it);

	if (!event.happened(Poller::EventType::write)
		|| client.ostate() != Client::OutputState::fetch
		|| _fetch_and_send(client) == IOStatus::failure)
		_drop(it);
}

void
Server::_to_graveyard(ClientIt it) {
	_graveyard.insert(_clients.extract(it));
}
