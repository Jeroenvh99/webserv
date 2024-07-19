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

std::string const&
Server::name() const noexcept {
	return (_name);
}

in_port_t
Server::port() const noexcept {
	Acceptor::Address	addr = acceptor().address();

	return (addr.port());
}

route::Route const&
Server::route() const noexcept {
	return (_route);
}

route::Location
Server::locate(std::filesystem::path const& path) const {
	return (_route.follow(path));
}

route::Location
Server::locate(URI const& uri) const {
	return (locate(uri.path()));
}

stdfs::path const&
Server::locate_errpage(http::Status status) const noexcept {
	auto const	it = _error_pages.find(status);

	if (it == _error_pages.end())
		return (no_errpage);
	return (it->second);
}

void
Server::add_httpredirect(std::string from, std::string to, bool permanent) {
	Server::Redirection redir;
	redir.from = URI(from);
	redir.to = URI(to);
	redir.permanent = permanent;
	_redirections.emplace_back(redir);
}

std::vector<Server::Redirection>
Server::getRedirections() const {
	return _redirections;
}

// Private methods

void
Server::_accept() {
	Client::Address	address;
	Client::Socket	socket = acceptor().accept(address);

	_elog.log(LogLevel::notice,
		"Established connection with peer at ", std::string(address), ".");
	Client::SocketBox	box = _poller.add(std::move(socket), poller_events, poller_mode);
	_clients.insert(std::make_pair(box, ClientImpl(address)));
}

void
Server::_drop(ClientMap::iterator it) {
	_elog.log(LogLevel::notice,
		"Terminated connection with peer at ",
		std::string(Client(*it).address()), ".");
	_poller.remove(it->first);
	_graveyard.erase(it);
}
