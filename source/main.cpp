#include "Route.hpp"

#include <iostream>
#include <stdexcept>

int
main() {
	Route	route("/var");

	route.allow_cgi("py");
	route.extend("/var/dir0").reset_cgi().redirect("/tmp/var/www/");
	route.extend("/var/dir1");

	try {
		route.extend("./dir0");
	} catch (std::invalid_argument& e) {
		std::cerr << e.what() << '\n';
	}

	RouteConfig	tip0 = route.follow("/var/dir1/file0");
	RouteConfig	tip1 = route.follow("/var/dir0/subdir0/lorem/ipsum/dolor");
	std::cout << tip0.directory_file() << '\n';
	std::cout << tip0.allows_cgi("php") << '\n';
	std::cout << tip1.allows_cgi("py") << ' ' << tip1.to() << '\n';

	route.set_directory_file("index.html");
	route.allow_cgi("php");
	std::cout << tip0.directory_file() << '\n';
	std::cout << tip0.allows_cgi("php") << '\n';

}
	

/*
int
main(int argc, char** argv) {
	if (argc > 2)
		return (std::cerr << "Usage: ./webserv [path_to_config]\n", 1);

	in_port_t const	port = (argc == 1) ? 1100 : std::stol(argv[1]); // temp

	try {
		Server	server(port);

		server.loop(5192);
	} catch (std::exception& e) {
		return (std::cerr << "webserv: " << e.what() << '\n', 1);
	}
	return (0);
}
*/
