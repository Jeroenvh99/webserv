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

route::Location
Server::locate(std::filesystem::path const& path) const {
	return (_route.follow(path));
}

std::string const&
Server::name() const noexcept {
	return (_name);
}

in_port_t
Server::port() const noexcept {
	Acceptor::Address	addr = acceptor().address();

	return (addr.port());
}

// Private methods

void
Server::_accept() {
	Client::Address	address;
	Client::Socket	socket = acceptor().accept(address);

	_elog.log(LogLevel::notice,
		"Connection established with peer at ", std::string(address));
	Client::SocketBox	box = _poller.add(std::move(socket), poller_events, poller_mode);
	_clients.insert(std::make_pair(box, ClientImpl(address)));
}

void
Server::_drop(ClientMap::iterator it) {
	_poller.remove(it->first);
	_graveyard.erase(it);
	_elog.log(LogLevel::notice, "Connection terminated.");
}
