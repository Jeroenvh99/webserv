#include "Server.hpp"

#include <iostream>

int
main() {
	try {
		Server	server(1100);

		server.loop(5192);
	} catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}
