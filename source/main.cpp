#include "Server.hpp"
#include "CGI.hpp"
#include "route.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

//static constexpr in_port_t	dfl_port = 1100;
//static constexpr int		dfl_backlog_size = 5192;
//static constexpr int		dfl_poller_timeout = 1000;

static size_t	_get_envsize(char const* const*);

int
main(int argc, char** argv, char** envp) {
	if (argc > 2)
		return (std::cerr << "Usage: ./webserv [path_to_config]\n", 1);

	route::Route	rt("/lorem/ipsum");
	rt.redirect("./www").allow_cgi("py");
	route::Location	loc0 = rt.follow("/lorem/ipsum/dolor/sit.py/amet/consectetur");
	route::Location	loc1 = rt.follow("/lorem/ipsum/dolor/sit.php/amet/consectetur");

	std::cout << loc0.from() << '\n' << loc0.to() << '\n' << loc0.path_info() << std::endl;
	std::cout << loc1.from() << '\n' << loc1.to() << '\n' << loc1.path_info() << std::endl;

	CGI::_envp = envp;
	CGI::_envsize = _get_envsize(envp);

	(void) argv;
 	CGI	cgi;
	std::stringstream		ss;
	ss.str("GET ./www/cgi/env.py HTTP/1.1\r\n\r\n");
	http::Request			req;
	http::Request::Parser	parser;

	parser.parse(ss, req);
	//cgi.launch(req);
	/*
	in_port_t const	port = (argc == 1) ? dfl_port : std::stol(argv[1]); // temp

	try {
		Server	server("localhost", port, dfl_backlog_size);

		while (true)
			server.process(dfl_poller_timeout);
	} catch (std::exception& e) {
		return (std::cerr << "webserv: " << e.what() << '\n', 1);
	}
	return (0);
	*/
}

static size_t	_get_envsize(char const* const* envp) {
	size_t	size = 0;

	while (envp[size++] != nullptr);
	return (size);
}
