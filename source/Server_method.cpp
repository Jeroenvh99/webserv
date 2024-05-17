#include "Server.hpp"

// Accessors

Server::Acceptor&
Server::acceptor() noexcept {
	return (static_cast<Acceptor&>(*_acceptor));
}

Server::Acceptor const&
Server::acceptor() const noexcept {
	return (static_cast<Acceptor const&>(*_acceptor));
}

// Private methods

void
Server::_accept() {
	Acceptor::Address	address;
	Client::Socket		socket = acceptor().accept(address);

	_elog.log(LogLevel::notice,
		"Connection established at ", std::string(address));
	Client::SocketBox	box = _poller.add(std::move(socket), poller_events, poller_mode);
	_clients.insert(std::make_pair(box, ClientData()));
}

void
Server::_drop(ClientMap::iterator it) {
	_poller.remove(it->first);
	_graveyard.erase(it);
	_elog.log(LogLevel::notice, "Connection terminated.");
}
