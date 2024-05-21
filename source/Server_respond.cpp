#include "http/Server.hpp"
#include <streambuf>

static http::Status	_get_file(std::string&, std::string const&);
static http::Status	_get_directory(std::string&, Route const&);

http::Response
Server::respond(http::Request const& req) {
	http::Status	status;
	std::string		body;

	switch (req.method()) {
	case Method::GET:
		status = get(body, req);
		break;
	case Method::POST:
		status = post(body, req);
		break;
	case Method::DELETE:
		status = delete_(body, req);
		break;
	}
	if (http::is_error(status))
		return (respond_error(status));
	return (http::Response(status, std::move(body)));
}

http::Response
Server::respond_error(http::Status error) {
	auto const	it = _error_pages.find(error);
	std::string	body;

	if (it != _error_pages.end()) {
		if (is_error(_get_file(body, it->second))) {
			error = http::Status::internal_error;
			body = http::to_string(error) + " " + http::description(error);
		}
	} else
		body = http::to_string(error) + " " + http::description(error);
	return (http::Response(error, std::move(body));
}

http::Status
Server::get(std::string& body, Request const& req) {
	Route const	route = route(req.uri());

	if (!route.allows_method(req.method()))
		return (http::Status::method_not_allowed);
	if (!std::filesystem::exists(route.to()))
		return (http::Status::not_found);
	if (std::filesystem::is_directory(route.to()))
		return (_get_directory(body, route));
	return (_get_file(body, route.to()));
}

http::Status
Server::post(std::string& body, Request const& req) {
	Route const	route = route(req.uri());

	if (!route.allows_method(req.method()))
		return (http::Status::method_not_allowed);
	/* implement */
	return (http::Status::ok); // DB: placeholder
}

http::Status
Server::delete_(std::string& body, Request const& req) {
	Route const	route = route(req.uri());

	if (!route.allows_method(req.method()))
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
}

static http::Status
_get_directory(std::string& body, Route const& route) {
	
