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

		for (size_t i = 0; i < serverconfigs.size(); i++) {
			std::ofstream accessFile;
			if(serverconfigs[i].accesslog.filename != "")
				accessFile.open(serverconfigs[i].accesslog.filename, std::ios::out);
			std::ostream& access = (serverconfigs[i].accesslog.filename != ""? accessFile : std::cout);
			std::ofstream errorFile;
			if(serverconfigs[i].errorlog.filename != "")
				errorFile.open(serverconfigs[i].errorlog.filename, std::ios::out);
			std::ostream& error = (serverconfigs[i].errorlog.filename != ""? errorFile : std::cerr);
			servers.emplace_back(Server(serverconfigs[i], dfl_backlog_size, access, error));
			if (i == serverconfigs.size() - 1) {
				while (true) {
					for (size_t j = 0; j < servers.size(); j++) {
						servers[j].process(dfl_poller_timeout);
					}
				}
			}
		}
	} catch (std::exception& e) {
		return (std::cerr << "webserv: " << e.what() << '\n', 1);
	}
	return (0);
}
