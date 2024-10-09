#include "Server.hpp"
#include "Poller.hpp"
#include "logging/logging.hpp"

using Elog = logging::ErrorLogger::Level;

// Accessors

Server::Acceptor&
Server::acceptor() noexcept {
	return (static_cast<Acceptor&>(*_acceptor));
}

Server::Acceptor const&
Server::acceptor() const noexcept {
	return (static_cast<Acceptor const&>(*_acceptor));
}

in_port_t
Server::port() const noexcept {
	Acceptor::Address	addr = acceptor().address();

	return (addr.port());
}

// Modifiers

void
Server::virtual_server_add(Config::Server config) {
	_possibleservers.emplace_back(VirtualServer(config));
}

VirtualServer const&
Server::virtual_server(std::string const& name) {
	for (auto const& vserv: _possibleservers)
		if (vserv.name() == name)
			return (vserv);
	return (_possibleservers[0]);
}

VirtualServer const&
Server::virtual_server(Client const& client) {
	std::string	hostname;

	try {
		hostname = client.request().headers().at("Host").csvalue();
		hostname.erase(hostname.find_last_of(':'));
		return (virtual_server(hostname));
	} catch (std::out_of_range&) {
		return (_possibleservers[0]);
	}
}

// Private methods

void
Server::_accept() {
	using EventType = webserv::Poller::EventType;

	Client::Address	address;
	Client::Socket	socket = acceptor().accept(address);

	logging::elog.log(Elog::notice,
		"Established connection with peer at ", address, ".");
	_clients.insert({
		g_poller.add(std::move(socket),
			{ EventType::read, EventType::write, EventType::hangup }),
			ClientImpl(address)
	});
}

void
Server::_drop(ClientMap::iterator it) {
	logging::elog.log(Elog::notice,
		"Terminated connection with peer at ", Client(*it).address(), ".");
	g_poller.remove(it->first);
	_graveyard.erase(it);
}
