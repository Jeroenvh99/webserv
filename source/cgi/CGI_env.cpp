#include "CGI.hpp"

#include <algorithm>

using route::Location;
 /*
static inline void	_add_location_vars(Environment&, Location const&, Server const&);
static inline void	_add_remote_vars(Environment&, Client const&);
static inline void	_add_content_vars(Environment&, http::Request const&);
static inline void	_add_request_vars(Environment&, http::Request const&);
static inline void	_add_header_vars(Environment&, http::Request const&);

Environment
CGI::env(Client const& client, Location const& loc, Server const& server) {
	Environment	env;

	env.reserve(base_env_size + client.request().header_count());
	_add_server_vars(env, server);
	_add_location_vars(env, client, server);
	_add_remote_vars(env, client);
	_add_request_vars(env, client.request());
	_add_header_vars(env, client.request());
	_add_headers(env, req);

	return (env);
}

// Helpers

static inline void
_add_request_vars(Environment& env, http::Request const& req) {
	URI const&	uri = req.uri();

	env.append("AUTH_TYPE", "");
	env.append("REQUEST_METHOD", http::to_string(req.method()));
	env.append("REQUEST_URI", std::string(uri));
	env.append("QUERY_STRING", uri.query());
	env.append("SERVER_PROTOCOL", http::to_string(req.version()));
}

static inline void
_add_location_vars(Environment& env, Location const& loc, Server const& serv) {
	if (loc.path_info()) {
		env.append("PATH_INFO", loc.path_info());
		env.append("PATH_TRANSLATED", serv.locate(loc.path_info()).to());
	}
	env.append("SCRIPT_NAME", loc.to());
}

static inline void	_add_remote_vars(Environment& env, Client const& client) {
	env.append("REMOTE_ADDR", "");
	env.append("REMOTE_HOST", "");
	env.append("REMOTE_IDENT", "");
	env.append("REMOTE_USER", "");
}

static inline void	_add_content_vars(Environment& env, http::Request const& req) {
	if (!req.has_body())
		return;
	env.append("CONTENT_LENGTH", std::to_string(req.body().size()));
	if (req.has_header("Content-Type"))
		env.append("CONTENT_TYPE", req.header("Content-Type"));
}

// DB: Headers queried for non-protocol variables and relating to authentication
// should be skipped.
static inline void	_add_header_vars(Environment& env, http::Request const& req) {
	for (auto const& hdr: req.headers())
		env.append(hdr);
}
*/
