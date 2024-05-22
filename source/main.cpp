#include "Server.hpp"
#include "Config.hpp"

#include <iostream>

int
main(int argc, char** argv) {
	if (argc > 2)
		return (std::cerr << "Usage: ./webserv [path_to_config]\n", 1);

	in_port_t const	port = (argc == 1) ? 1100 : std::stol(argv[1]); // temp

	try {
		std::string file;
		if (argc == 2) {
			file = argv[1];
		} else {
			file = "configs/default.conf";
		}
		Config	conf(file);
		Server	server(port);

		server.loop(5192);
	} catch (std::exception& e) {
		return (std::cerr << "webserv: " << e.what() << '\n', 1);
	}
	return (0);
}
