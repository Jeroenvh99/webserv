#include "Server.hpp"

Server::Acceptor&
Server::acceptor() noexcept {
	return (static_cast<Acceptor&>(*_acceptor));
}

Server::Acceptor const&
Server::acceptor() const noexcept {
	return (static_cast<Acceptor const&>(*_acceptor));
}
