#include "Environment.hpp"
#include "Server.hpp"
#include "Client.hpp"

#include <cstring>

using route::Location;
using route::Route;

using Container = Environment::Container;

// static inline void	_add_server(Container&, Server const&);
// static inline void	_add_remote(Container&, Client const&);
// static inline void	_add_location(Container&, Location const&, Route const&);
// static inline void	_add_request(Container&, http::Request const&);
// static inline void	_add_auth(Container&, http::Request const&);
// static inline void	_add_content(Container&, http::Request const&);
// static inline void	_add_headers(Container&, http::Request const&);

void
Environment::build() {
	_ctr.clear();
	_ctr.reserve(dfl_size + _client.request().headers().size());

	append("SERVER_NAME", _vserver.name()),
	append("SERVER_PORT", std::to_string(_server.port()));

	if (_location.path_info().size()) {
		append("PATH_INFO", _location.path_info());
		append("PATH_TRANSLATED", _location.translate(_vserver.route()).to());
	}
	append("SCRIPT_NAME", _location.to());

	append("REMOTE_ADDR", std::string(_client.address()));
	append("REMOTE_HOST", std::string(_client.address())); // DB: If getnameinfo() were an allowed function, this could be done too.
	append("REMOTE_IDENT", ""); // unused

	http::Request const&	req = _client.request();

	append("REQUEST_METHOD", http::to_string(req.method()));
	append("REQUEST_URI", std::string(req.uri()));
	append("QUERY_STRING", req.uri().query());
	append("SERVER_PROTOCOL", http::to_string(req.version()));

	append("REMOTE_USER", ""); // implement
	append("AUTH_TYPE", ""); // implement

	if (req.headers().contains("Content-Type"))
		append("CONTENT_TYPE", "<placeholder>");
	if (req.headers().contains("Content-Length"))
		append("CONTENT_LENGTH", "<placeholder>");

	for (auto const& kv: req.headers()) {
		http::Header const	hdr(kv);
		append(hdr);
	}

	_build_cenv();
}

void
Environment::_build_cenv() {
	size_t const	total_size = _parent_env_size + static_size + _ctr.size();
	delete[] _cenv;
	_cenv = new char*[total_size + 1];
	char** const&	cenv_offset = _cenv + _parent_env_size;
	char** const&	static_offset = cenv_offset + static_size;

	std::memmove(_cenv, _parent_env, _parent_env_size * sizeof(char*));
	std::memmove(cenv_offset, static_env.data(), static_size * sizeof(char*));
	for (size_t i = 0; i < _ctr.size(); ++i)
		static_offset[i] = _ctr.at(i).data();
	_cenv[total_size] = nullptr;
}

// Helpers

// static inline void
// _add_server(Container& ctr, Server const& serv) {

// }

// static inline void
// _add_location(Container& ctr, Location const& loc, Route const& root) {

// }

// static inline void
// _add_remote(Container& ctr, Client const& client) {

// }

// static inline void
// _add_request(Container& ctr, http::Request const& req) {
// 	URI const&	uri = req.uri();


// 	_add_auth(ctr, req);
// 	_add_content(ctr, req);
// 	_add_headers(ctr, req);
// }

// static inline void
// _add_auth(Container& ctr, http::Request const&) {

// }

// static inline void
// _add_content(Container& ctr, http::Request const& req) {

// }

// // DB: Headers queried for non-protocol variables and relating to authentication
// // should be skipped.
// static inline void
// _add_headers(Container& ctr, http::Request const& req) {

// }