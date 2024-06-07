#include "Server.hpp"

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

using namespace logging;

Server::Server(std::string const& name, in_port_t port, int backlog_size,
		std::ostream& alog, std::ostream& elog): // remove this once config parser is done
	_name(name),
	_poller(),
	_acceptor(),
	_clients(),
	_route("/"),
	_error_pages(),
	_alog(alog, Format{
		Variable("["), Variable(Variable::Type::time_local), Variable("]")
	}),
	_elog(elog, ErrorLogger::Level::debug) {
	_acceptor = _poller.add(Acceptor(Acceptor::Address(port, INADDR_ANY)),
							{Poller::EventType::read},
							{Poller::Mode::edge_triggered});
	// if this can be moved to the initializer list, it'd be great
	_route.allow_method(http::Method::GET)
		.redirect("./www")
		.set_directory_file("index.html");
	_route.extend("/cgi")
		.forbid_directory()
		.allow_cgi("py");
	_route.extend("/stuff")
		.allow_method(http::Method::POST);
	acceptor().listen(backlog_size);
}

Server::Server(Server&& serv) {
	this = &serv;
}
