#include "Server.hpp"
#include "Environment.hpp"
#include "route.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

static constexpr in_port_t	dfl_port = 1100;
static constexpr int		dfl_backlog_size = 5192;
static constexpr int		dfl_poller_timeout = 1000;

static size_t	_get_cenvsize(char const* const*);

int
main(int argc, char** argv, char** envp) {
	Environment::_cenv = envp;
	Environment::_cenv_size = _get_cenvsize(envp);

	if (argc > 2)
		return (std::cerr << "Usage: ./webserv [path_to_config]\n", 1);

	in_port_t const	port = (argc == 1) ? dfl_port : std::stol(argv[1]); // temp

	try {
		Server	server("localhost", port, dfl_backlog_size);

		while (true)
			server.process(dfl_poller_timeout);
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
