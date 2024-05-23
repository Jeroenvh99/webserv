#include "Server.hpp"
#include "Config.hpp"

#include <iostream>

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
		const std::vector<Config::Server> servers = conf.getServers();
		for (Config::Server config : servers) {
			Server	server(config.port);
			server.loop(5192);
		}
	} catch (std::exception& e) {
		return (std::cerr << "webserv: " << e.what() << '\n', 1);
	}
	return (0);
}
