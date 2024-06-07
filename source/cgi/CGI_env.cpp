#include "CGI.hpp"

static std::string	_make_envstr(std::string const&, std::string const&);
static void			_add_headers(Environment&, Request const&);

CGI::Environment
CGI::env(http::Request const& req) {
	Environment	env;

	env.reserve(17 + req.headers().size());
	env.push_back("SERVER_SOFTWARE=Webserv/1.0");
	env.push_back("SERVER_NAME="); // todo
	env.push_back("GATEWAY_INTERFACE=CGI/1.1");

	env.push_back(_make_envstr("SERVER_PROTOCOL", http::to_string(req.version())));
	env.push_back("SERVER_PORT="); // todo

	env.push_back(_make_envstr("REQUEST_METHOD", http::to_string(req.method())));
	env.push_back(_make_envstr("REQUEST_URI", std::string(req.uri())));
	env.push_back(_make_envstr("PATH_INFO", req.uri().path_info));
	env.push_back(_make_envstr("PATH_TRANSLATED", req.))
	env.push_back(_make_envstr("SCRIPT_NAME", req.uri().path()));
	env.push_back("REDIRECT_STATUS=200");
	if (req.has_header("Content-Type"))
		env.push_back(_make_envstr("CONTENT_TYPE", req.header("Content-Type")));
	_add_headers(env, req);
}

static std::string
_make_envstr(std::string const& key, std::string const& value) {
	return (key.append("=").append(value));
}

static void
_add_headers(Environment& env, Request const& req) {
}
