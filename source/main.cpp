#include "Server.hpp"
#include "Environment.hpp"
#include "Poller.hpp"
#include "route.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

static constexpr in_port_t	dfl_port = 1100;
static constexpr int		backlog_size = 1024;

static size_t	_get_cenvsize(char const* const*);

int
main(int argc, char** argv, char** envp) {
	Environment::_parent_env = envp;
	Environment::_parent_env_size = _get_cenvsize(envp);

	if (argc > 2)
		return (std::cerr << "Usage: ./webserv [path_to_config]\n", 1);

	in_port_t const	port = (argc == 1) ? dfl_port : std::stol(argv[1]); // temp

	try {
		Server	server("localhost", port, backlog_size);

		while (true) {
			g_poller.wait();
			server.process();
		}
	} catch (std::exception& e) {
		return (std::cerr << "webserv: " << e.what() << '\n', 1);
	}
	return (0);
}

static size_t	_get_cenvsize(char const* const* envp) {
	size_t	size = 0;

	while (envp[size] != nullptr)
		++size;
	return (size);
}
