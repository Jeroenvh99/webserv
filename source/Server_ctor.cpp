#include "Server.hpp"
#include "Poller.hpp"

/*
Server::Server(RouteConfig&& config):
	_poller(),
	_acceptor(_poller.add(Acceptor::Address(config.port, INADDR_ANY)),
							{Poller::EventType::read},
							{Poller::Mode::edge_triggered}), // might not be required
	_clients(),
	_routes(std::move(config.routes)),
	_error_pages(std::move(config.error_pages)),
	_buffer() {}
*/

stdfs::path const	VirtualServer::no_errpage = "";

Server::Server(Config::Server config, int backlog_size):
	_acceptor(g_poller.add(Acceptor(Acceptor::Address(config.port, INADDR_ANY)),
							{webserv::Poller::EventType::read})),
	_clients() {
	acceptor().listen(backlog_size);
	addVirtualServer(config);
}
