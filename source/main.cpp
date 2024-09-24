#include "Server.hpp"
#include "Config.hpp"
#include "Environment.hpp"
#include "Poller.hpp"
#include "logging/logging.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>

using Servers = std::vector<Server>;

static constexpr int	default_backlog = 5192;

static size_t	get_envsize(char const* const*);
static void		configure(Servers&, char const*);
static void		configure_logs(Config const&);
static void		configure_servers(Servers&, Config const&);

logging::AccessLogger	logging::alog;
logging::ErrorLogger	logging::elog;

int
main(int argc, char** argv, char** envp) {
	Environment::_parent_env = envp;
	Environment::_parent_env_size = get_envsize(envp);

	if (argc > 2) {
		std::cerr << "Usage: " << argv[0] << " [path_to_config]\n";
		return (EXIT_FAILURE);
	}
	try {
		Servers servers;
		
		configure(servers, (argc == 2) ? argv[1] : "configs/default.conf");
		while (true) {
			g_poller.wait();
			for (auto& server: servers)
				server.process();
		}
	} catch (std::exception& e) {
		std::cerr << "webserv: " << e.what() << '\n';
		return (EXIT_FAILURE);
	}
    std::cout << "webserv: exiting\n";
	return (EXIT_SUCCESS);
}

static size_t
get_envsize(char const* const* envp) {
	size_t	size = 0;

	while (envp[size] != nullptr)
		++size;
	return (size);
}

static void
configure(Servers& servers, char const* cpath) {
	std::string		path(cpath);
	Config const	conf(path);

	configure_logs(conf);
	configure_servers(servers, conf);
}

static void
configure_logs(Config const&) {
	// for both logging::alog and logging::elog
	// check if config specifies a path to a file
	// if so: call .attach_file(path), this should throw an error if the file couldn't be opened
	// else, do nothing (loggers remain attached to standard streams)
}

static void
configure_servers(Servers& servers, Config const& conf) {
	for (auto const& config: conf.getServers()) {
		auto	it = std::find_if(servers.begin(), servers.end(),
			[config](Servers::value_type const& server) {
				return (server.port() == config.port);
			}
		);
		
		if (it == servers.end())
			servers.emplace_back(config, default_backlog);
		else
			it->addVirtualServer(config);
	}
}