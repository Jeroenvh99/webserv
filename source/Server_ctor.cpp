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

using logging::ErrorLogger;
using logging::Format;
using logging::Variable;

stdfs::path const	VirtualServer::no_errpage = "";

Server::Server(Config::Server config, int backlog_size,
		std::ostream& alog, std::ostream& elog): // remove this once config parser is done
	_acceptor(g_poller.add(Acceptor(Acceptor::Address(config.port, INADDR_ANY)),
							{webserv::Poller::EventType::read})),
	_clients(),
	_alog(alog, Format{
		Variable("["), Variable(Variable::Type::time_local), Variable("]")
	}),
	_elog(elog, config.errorlog.level) {
	acceptor().listen(backlog_size);
    _elog.log(ErrorLogger::Level::notice, "Server ", _name, " listening on port ", config.port);
}
