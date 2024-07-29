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

using logging::AccessLogger;
using logging::ErrorLogger;
using logging::Format;
using logging::Variable;

stdfs::path const	Server::no_errpage = "";

Server::Server(Config::Server config, int backlog_size,
		std::ostream& alog, std::ostream& elog): // remove this once config parser is done
	_name(config.servername),
	_acceptor(g_poller.add(Acceptor(Acceptor::Address(port, INADDR_ANY)),
							{webserv::Poller::EventType::read})),
	_clients(),
	_route("/"),
	_error_pages(),
	_alog(alog, Format{
		Variable("["), Variable(Variable::Type::time_local), Variable("]")
	}),
	_elog(elog, config.errorlog.level) {
	for (size_t i = 0; i < config.redirections.size(); i++) {
		add_httpredirect(config.redirections[i].from, config.redirections[i].to, config.redirections[i].permanent);
	}
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
				_route.seek(loc.paths[0]).set_directory_file(loc.index);
			}
			for (int i = 0; i < static_cast<int>(http::Method::NONE); i++) {
				if (loc.allowedmethods[i] != http::Method::NONE) {
					_route.seek(loc.paths[0]).allow_method(loc.allowedmethods[i]);
				} else {
					_route.seek(loc.paths[0]).disallow_method(loc.allowedmethods[i]);
				}
			}
			for (std::string cgitype : loc.allowedcgi) {
				_route.seek(loc.paths[j]).allow_cgi(cgitype);
			}
		}
	}
	for (auto& errorpage : config.errorpages) {
		_error_pages.insert(std::pair<http::Status, std::filesystem::path>(static_cast<http::Status>(errorpage.first), errorpage.second));
	}
	acceptor().listen(backlog_size);
}
