#include "Server.hpp"
#include "Poller.hpp"

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

in_port_t
Server::port() const noexcept {
	Acceptor::Address	addr = acceptor().address();

	return (addr.port());
}

route::Route const&
VirtualServer::route() const noexcept {
	return (_route);
}

route::Location
VirtualServer::locate(std::filesystem::path const& path) const {
	return (_route.follow(path));
}

route::Location
VirtualServer::locate(URI const& uri) const {
	return (locate(uri.path()));
}

stdfs::path const&
VirtualServer::locate_errpage(http::Status status) const noexcept {
	auto const	it = _error_pages.find(status);

	if (it == _error_pages.end())
		return (no_errpage);
	return (it->second);
}

void
VirtualServer::add_httpredirect(std::string from, std::string to, bool permanent) {
	VirtualServer::Redirection redir;
	redir.from = URI(from);
	redir.to = URI(to);
	redir.permanent = permanent;
	_redirections.emplace_back(redir);
}

std::vector<VirtualServer::Redirection>
VirtualServer::getRedirections() const {
	return _redirections;
}

int
VirtualServer::getMaxBodySize() const {
	return _maxbodysize;
}

void
Server::addVirtualServer(Config::Server config) {
	_possibleservers.emplace_back(VirtualServer(config));
}

VirtualServer const&	Server::searchVirtualServer(std::string name) {
	for (size_t i = 0; i < _possibleservers.size(); i++) {
		if (_possibleservers[i].name() == name) {
			return (_possibleservers[i]);
		}
	}
	return (_possibleservers[0]);
}

// Private methods

void
Server::_accept() {
	using EventType = webserv::Poller::EventType;

	Client::Address	address;
	Client::Socket	socket = acceptor().accept(address);

	_elog.log(LogLevel::notice,
		"Established connection with peer at ", std::string(address), ".");
	_clients.insert({
		g_poller.add(std::move(socket),
			{ EventType::read, EventType::write, EventType::hangup }),
			ClientImpl(address)
	});
}

void
Server::_drop(ClientMap::iterator it) {
	_elog.log(LogLevel::notice,
		"Terminated connection with peer at ",
		std::string(Client(*it).address()), ".");
	g_poller.remove(it->first);
	_graveyard.erase(it);
}
