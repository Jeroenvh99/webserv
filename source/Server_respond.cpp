#include "Server.hpp"

#include <filesystem>
#include <fstream>
#include <streambuf>

static http::Status	_get_file(std::string&, std::string const&);
static http::Status	_get_directory(std::string&, Route const&);
static http::Status	_list_directory(std::string&, std::string const&);

http::Response
Server::respond(http::Request const& req) const {
	http::Status	status;
	std::string		body;

	switch (req.method()) {
	case http::Method::GET:
		status = get(body, req);
		break;
	case http::Method::POST:
		status = post(body, req);
		break;
	case http::Method::DELETE:
		status = delete_(body, req);
		break;
	default:
		return (respond_error(http::Status::method_not_allowed));
	}
	if (http::is_error(status))
		return (respond_error(status));
	return (http::Response(std::move(body), status));
}

http::Response
Server::respond_error(http::Status error) const {
	auto const	it = _error_pages.find(error);
	std::string	body;

	if (it != _error_pages.end()) {
		if (is_error(_get_file(body, it->second))) {
			error = http::Status::internal_error;
			body = http::to_string(error) + " " + http::description(error);
		}
	} else
		body = http::to_string(error) + " " + http::description(error);
	return (http::Response(std::move(body), error));
}

http::Status
Server::get(std::string& body, http::Request const& req) const {
	Route const	rt = route(req.uri());

	if (!rt.allows_method(req.method()))
		return (http::Status::method_not_allowed);
	if (!std::filesystem::exists(rt.to()))
		return (http::Status::not_found);
	if (std::filesystem::is_directory(rt.to()))
		return (_get_directory(body, rt));
	return (_get_file(body, rt.to()));
}

http::Status
Server::post(std::string&, http::Request const& req) const {
	Route const	rt = route(req.uri());

	if (!rt.allows_method(req.method()))
		return (http::Status::method_not_allowed);
	/* implement */
	return (http::Status::ok); // DB: placeholder
}

http::Status
Server::delete_(std::string&, http::Request const& req) const {
	Route const	rt = route(req.uri());

	if (!rt.allows_method(req.method()))
		return (http::Status::method_not_allowed);
	/* implement */
	return (http::Status::ok); // DB: placeholder
}

static http::Status
_get_file(std::string& body, std::string const& path) {
	std::ifstream		file(path);
	std::stringstream	ss;

	if (!file.is_open())
		return (http::Status::forbidden);
	ss << file.rdbuf();
	body = ss.str();
	return (http::Status::ok);
}

static http::Status
_get_directory(std::string& body, Route const& rt) {
	if (rt.default_file() == Route::directory_listing)
		return (_list_directory(body, rt.to()));
	return (_get_file(body, rt.default_file()));
}

static http::Status
_list_directory(std::string& body, std::string const& path) {
	std::ostringstream	oss;

	for (auto const& entry: std::filesystem::directory_iterator(path))
		oss << entry.path() << '\n';
	body = std::move(oss.str());
	return (http::Status::ok);
}
