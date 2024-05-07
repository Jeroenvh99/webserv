#include "Server.hpp"

/*
Server::Server(Config&& config):
	_poller(),
	_acceptor(_poller.add(Acceptor::Address(config.port, INADDR_ANY)),
							{Poller::EventType::read},
							{Poller::Mode::edge_triggered}), // might not be required
	_clients(),
	_routes(std::move(config.routes)),
	_error_pages(std::move(config.error_pages)),
	_buffer() {}
*/

Server::Server(in_port_t port): // remove this once config parser is done
	_poller(),
	_acceptor(),
	_clients(),
	_routes(),
	_error_pages(),
	_buffer() {
	_acceptor = _poller.add(Acceptor(Acceptor::Address(port, INADDR_ANY)),
							{Poller::EventType::read},
							{Poller::Mode::edge_triggered});
	// if this can be moved to the initializer list, it'd be great
}
