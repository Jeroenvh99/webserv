#include "Server.hpp"
#include "Poller.hpp"
#include "logging/logging.hpp"

using webserv::Poller;
using Elog = logging::ErrorLogger::Level;

void
Server::process() {
	for (auto const& [handle, event]: g_poller) {
		if (handle == _acceptor)
			_accept();
		else {
			ClientIt	it = _clients.find(handle);

			if (it == _clients.end()) {		// not an active client
				it = _graveyard.find(handle);
				if (it == _graveyard.end())	// CGI or belongs to another server
					continue;
				_process_graveyard(event, it);
			} else
				_process_core(event, it);
		}
	}
}

void
Server::_process_core(Poller::Event const& event, ClientIt it) {
	Client	client(*it);

	if (event.happened(Poller::EventType::hangup)) {
		logging::elog.log(Elog::notice, client.address(),
			": Connection closed by peer.");
		_to_graveyard(it);
	}
	else if (event.happened(Poller::EventType::read)
		&& _process_read(client) == IOStatus::failure)
		_to_graveyard(it);
	else if (event.happened(Poller::EventType::write)
		&& _process_write(client) == IOStatus::failure)
		_to_graveyard(it);
}

Server::IOStatus
Server::_process_read(Client& client) {
	switch (client.istate()) {
	case Client::InputState::parse_request:
		return (_parse_request(client));
	case Client::InputState::deliver:
		return (_recv_and_deliver(client));
	case Client::InputState::dechunk:
		return (_recv_dechunk_and_deliver(client));
	case Client::InputState::closed:
		return (IOStatus::failure);
	}
	__builtin_unreachable();
}

Server::IOStatus
Server::_process_write(Client& client) {
	switch (client.ostate()) {
	case Client::OutputState::parse_response:
		return (_parse_response(client));
	case Client::OutputState::fetch:
		return (_fetch_and_send(client));
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
