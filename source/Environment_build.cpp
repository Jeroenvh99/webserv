#include "Environment.hpp"
#include "Client.hpp"

using route::Location;
using route::Route;

using Container = Environment::Container;

static inline void	_add_server(Container&, Server const&);
static inline void	_add_remote(Container&, Client const&);
static inline void	_add_location(Container&, Location const&, Route const&);
static inline void	_add_request(Container&, http::Request const&);
static inline void	_add_auth(Container&, http::Request const&);
static inline void	_add_content(Container&, http::Request const&);
static inline void	_add_headers(Container&, http::Request const&);

void
Environment::build() {
	_clear();
	_reserve(dfl_size + client.request().header_count());
	_add_server(_ctr, _server);
	_add_remote(_ctr, _client);
	_add_location(_ctr, _location, _server.route());
	_add_request(_ctr, _client.request());
	_build_cenv();
}

void
Environment::_build_cenv() {
	size_t const	total_size = _cenv_size + static_size + _ctr.size();
	delete[] _cenv;
	_cenv = new char*[total_size + 1];
	char** const&	cenv_offset = p + _cenv_size;
	char** const&	static_offset = cenv_offset + static_size;

	::memmove(p, _cenv, _cenv_size * sizeof(char*));
	::memmove(cenv_offset, static_env.data(), static_size * sizeof(char*));
	for (size_t i = 0; i < _ctr.size(); ++i)
		static_offset[i] = _ctr.at(i).data();
	p[total_size] = nullptr;
}

// Helpers

static inline void
_add_server(Container& ctr, Server const& serv) {
	append("SERVER_NAME", serv.name()),
	append("SERVER_PORT", std::to_string(serv.port));
}

static inline void
_add_location(Container& ctr, Location const& loc, Route const& root) {
	if (loc.path_info().size()) {
		append("PATH_INFO", loc.path_info());
		append("PATH_TRANSLATED", loc.translate(root).to());
	}
	append("SCRIPT_NAME", loc.to());
}

static inline void
_add_remote(Container& ctr, Client const& client) {
	append("REMOTE_ADDR", std::string(client.address()));
	append("REMOTE_HOST", std::string(client.address())); // DB: If getnameinfo() were an allowed function, this could be done too.
	append("REMOTE_IDENT", ""); // unused
}

static inline void
_add_request(Container& ctr, http::Request const& req) {
	URI const&	uri = req.uri();

	append("REQUEST_METHOD", http::to_string(req.method()));
	append("REQUEST_URI", std::string(uri));
	append("QUERY_STRING", uri.query());
	append("SERVER_PROTOCOL", http::to_string(req.version()));
	_add_auth(ctr, req);
	_add_content(ctr, req);
	_add_headers(ctr, req);
}

static inline void
_add_auth(Container& ctr, http::Request const&) {
	append("REMOTE_USER", ""); // implement
	append("AUTH_TYPE", ""); // implement
}

static inline void
_add_content(Container& ctr, http::Request const& req) {
	if (req.has_header("Content-Type"))
		append("CONTENT_TYPE", req.header("Content-Type"));
	if (req.has_header("Content-Length"))
		append("CONTENT_LENGTH", req.header("Content-Length"));
}

// DB: Headers queried for non-protocol variables and relating to authentication
// should be skipped.
static inline void
_add_headers(Container& ctr, http::Request const& req) {
	for (auto const& hdr: req.headers())
		append(hdr);
}