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

using logging::AccessLogger;
using logging::ErrorLogger;
using logging::Format;
using logging::Variable;

Server::Server(Config::Server config, int backlog_size,
		std::ostream& alog, std::ostream& elog): // remove this once config parser is done
	_name(config.servername),
	_poller(),
	_acceptor(),
	_clients(),
	_route("/"),
	_error_pages(),
	_alog(alog, Format{
		Variable("["), Variable(Variable::Type::time_local), Variable("]")
	}),
	_elog(elog, config.errorlog.level) {
	_acceptor = _poller.add(Acceptor(Acceptor::Address(config.port, INADDR_ANY)),
							{Poller::EventType::read},
							{Poller::Mode::edge_triggered});
	// if this can be moved to the initializer list, it'd be great
	// for (int i = 0; i < static_cast<int>(http::Method::NONE); i++) {
	// 	if (config.allowedmethods[i] != http::Method::NONE) {
	// 		_route.allow_method(config.allowedmethods[i]);
	// 	}
	// }
	_route.allow_method(http::Method::POST);
	// for (Config::Location loc : config.locations) {
	// 	_route.extend(loc.path);
	// 	for (int i = 0; i < http::Method::NONE; i++) {
	// 		if (loc.allowedmethods[i] != http::Method::NONE) {
	// 			_route.allow_method(loc.allowedmethods[i]);
	// 		}
	// 	}
	// }
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
