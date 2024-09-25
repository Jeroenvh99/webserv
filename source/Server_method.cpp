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

VirtualServer::VirtualServer(Config::Server config) :
_name(config.servername),
_route("/"),
_port(config.port),
_maxbodysize(config.maxbodysize) {
	for (size_t i = 0; i < config.redirections.size(); i++) {
		add_httpredirect(config.redirections.at(i).from, config.redirections.at(i).to, config.redirections.at(i).permanent);
	}
	for (Config::Location loc : config.locations) {
		for (size_t i = 0; i < loc.paths.size(); i++) {
			_route.extend(loc.paths.at(i))
				.redirect(loc.root)
				.list_directory();
			if (!loc.index.empty()) {
				_route.seek(loc.paths.at(i)).set_directory_file(loc.index);
			}
			for (size_t j = 0; j < loc.allowedmethods.size(); j++) {
				if (loc.allowedmethods.at(j) != http::Method::NONE) {
					_route.seek(loc.paths.at(i)).allow_method(loc.allowedmethods.at(j));
				} else {
					_route.seek(loc.paths.at(i)).disallow_method(loc.allowedmethods.at(j));
				}
			}
			for (size_t j = 0; j < loc.allowedcgi.size(); j++) {
				_route.seek(loc.paths.at(i)).allow_cgi(loc.allowedcgi[j]);
			}
		}
	}
	for (auto& errorpage : config.errorpages) {
		_error_pages.insert(std::pair<http::Status, std::filesystem::path>(static_cast<http::Status>(errorpage.first), errorpage.second));
	}
}

std::string const&
VirtualServer::name() const noexcept {
	return (_name);
}

int const&
VirtualServer::port() const noexcept {
	return (_port);
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
	std::string	hostname = client.request().headers().at("Host").csvalue();

	hostname.erase(hostname.find_last_of(':'));
	return (virtual_server(hostname));
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
