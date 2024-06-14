#include "Environment.hpp"
#include "Client.hpp"

static inline void	_add_location(Environment&, route::Location const&);
static inline void	_add_remote(Environment&, Client const&);
static inline void	_add_request(Environment&, http::Request const&);
static inline void	_add_auth(Environment&, http::Request const&);
static inline void	_add_content(Environment&, http::Request const&);
static inline void	_add_header(Environment&, http::Request const&);

// Static variables

Environment::StaticContainer const	Environment::static_env{
	"SERVER_SOFTWARE=Webserv/1.0",
	"GATEWAY_INTERFACE=CGI/1.1"};

char**	Environment::_cenv;
size_t	Environment::_cenv_size;

Environment::Environment(std::initializer_list<std::string> ilist):
	_ctr(ilist) {}

Environment::Environment(Environment const& base, route::Location const& loc, Client const& client):
	_ctr(base.ctr()) {
	_ctr.reserve(dfl_size + client.request().header_count());

	_add_location(*this, loc);
	_add_remote(*this, client);
	_add_request(*this, client.request());
}

// Helpers

static inline void
_add_location(Environment& env, route::Location const& loc) {
	if (loc.path_info().size()) {
		env.append("PATH_INFO", loc.path_info());
		env.append("PATH_TRANSLATED", loc.translate().to());
	}
	env.append("SCRIPT_NAME", loc.to());
}

static inline void
_add_remote(Environment& env, Client const& client) {
	env.append("REMOTE_ADDR", std::string(client.address()));
	env.append("REMOTE_HOST", std::string(client.address())); // DB: If getnameinfo() were an allowed function, this could be done too.
	env.append("REMOTE_IDENT", ""); // unused
}

static inline void
_add_request(Environment& env, http::Request const& req) {
	URI const&	uri = req.uri();

	env.append("REQUEST_METHOD", http::to_string(req.method()));
	env.append("REQUEST_URI", std::string(uri));
	env.append("QUERY_STRING", uri.query());
	env.append("SERVER_PROTOCOL", http::to_string(req.version()));
	_add_auth(env, req);
	_add_content(env, req);
	_add_header(env, req);
}

static inline void
_add_auth(Environment& env, http::Request const&) {
	env.append("REMOTE_USER", ""); // implement
	env.append("AUTH_TYPE", ""); // implement
}

static inline void
_add_content(Environment& env, http::Request const& req) {
	if (!req.has_body())
		return;
	env.append("CONTENT_LENGTH", std::to_string(req.body().size()));
	if (req.has_header("Content-Type"))
		env.append("CONTENT_TYPE", req.header("Content-Type"));
}

// DB: Headers queried for non-protocol variables and relating to authentication
// should be skipped.
static inline void
_add_header(Environment& env, http::Request const& req) {
	for (auto const& hdr: req.headers())
		env.append(hdr);
}