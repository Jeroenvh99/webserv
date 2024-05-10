#include "Server.hpp"

#include <iostream>

int
main(int argc, char** argv) {
	if (argc > 2) {
		std::cerr << "Usage: ./webserv [path_to_config]\n";
		return (1);
	}
	in_port_t const	port = (argc == 1) ? 1100 : std::stol(argv[1]); // temp

	try {
		Server	server(port);
		server.loop(5192);
	} catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	return (0);
}
