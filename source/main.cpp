#include "Server.hpp"
#include "Config.hpp"

#include <iostream>
#include <stdexcept>

static constexpr int		dfl_backlog_size = 5192;
static constexpr int		dfl_poller_timeout = 1000;

int
main(int argc, char** argv) {
	if (argc > 2)
		return (std::cerr << "Usage: ./webserv [path_to_config]\n", 1);
	try {
		std::string file;
		if (argc == 2) {
			file = argv[1];
		} else {
			file = "configs/default.conf";
		}
		Config	conf(file);
		const std::vector<Config::Server> serverconfigs = conf.getServers();
		std::vector<Server> servers;

		for (Config::Server config : serverconfigs) {
			std::ofstream access(config.accesslog.filename);
			std::ofstream error(config.errorlog.filename);
			servers.push_back(Server(config, dfl_backlog_size, access, error));
		}
		while (true) {
			for (size_t i = 0; i < serverconfigs.size(); i++) {
				servers[i].process(dfl_poller_timeout);
			}
		}
	} catch (std::exception& e) {
		return (std::cerr << "webserv: " << e.what() << '\n', 1);
	}
	return (0);
}
