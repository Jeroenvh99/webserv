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
	_acceptor = _poller.add(Acceptor(Acceptor::Address(static_cast<in_port_t>(config.port), INADDR_ANY)),
							{Poller::EventType::read},
							{Poller::Mode::edge_triggered});
	// if this can be moved to the initializer list, it'd be great
	_route.redirect("./");
	for (int i = 0; i < static_cast<int>(http::Method::NONE); i++) {
		if (config.allowedmethods[i] != http::Method::NONE) {
			_route.allow_method(config.allowedmethods[i]);
		} else {
			_route.disallow_method(config.allowedmethods[i]);
		}
	}
	for (Config::Location loc : config.locations) {
		for (size_t j = 0; j < loc.paths.size(); j++) {
			_route.extend(loc.paths[j])
				.redirect(loc.root)
				.list_directory();
			if (!loc.index.empty()) {
				_route.set_directory_file(loc.index);
			}
			for (int i = 0; i < static_cast<int>(http::Method::NONE); i++) {
				if (loc.allowedmethods[i] != http::Method::NONE) {
					_route.allow_method(loc.allowedmethods[i]);
				} else {
					_route.disallow_method(loc.allowedmethods[i]);
				}
			}
			if (loc.paths[0].find("cgi") != std::string::npos) {
				_route.allow_cgi("py");
			}
		}
	}
	for (auto& errorpage : config.errorpages) {
		_error_pages.insert(std::pair<http::Status, std::filesystem::path>(static_cast<http::Status>(errorpage.first), errorpage.second));
	}
	acceptor().listen(backlog_size);
}
