#include "Server.hpp"

#include <filesystem>
#include <fstream>
#include <streambuf>

static http::Status	_get_file(std::string&, route::Path const&);
static http::Status	_get_directory(std::string&, route::Location const&);
static http::Status	_list_directory(std::string&, route::Path const&);

http::Response
Server::respond(http::Request const& req) {
	http::Status		status;
	std::string			body;

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
		return (respond_error(http::Status::not_implemented));
	}
	if (http::is_error(status))
		return (respond_error(status));
	return (http::Response(std::move(body), status));
}

http::Response
Server::respond_error(http::Status error) {
	auto const	it = _error_pages.find(error);
	std::string	body;

	if (it != _error_pages.end()) { // an error page is defined for this status
		if (is_error(_get_file(body, it->second))) // but couldn't be accessed
			return (http::Response(http::Status::internal_error));
		return (http::Response(std::move(body), error));
	}
	return (http::Response(error));
}

http::Status
Server::get(std::string& body, http::Request const& req) {
	route::Location const	loc = locate(req.uri().path());

	if (!loc.allows_method(req.method()))
		return (http::Status::method_not_allowed);
	if (!std::filesystem::exists(loc.to()))
		return (http::Status::not_found);
	if (std::filesystem::is_directory(loc.to()))
		return (_get_directory(body, loc));
	return (_get_file(body, loc.to()));
}

http::Status
Server::post(std::string&, http::Request const& req) {
	route::Location const	loc = locate(req.uri().path());

	if (!loc.allows_method(req.method())) {
		_elog.log(LogLevel::error,
			"Method ", http::to_string(req.method()),
			" not allowed at ", loc.to(), ".");
		return (http::Status::method_not_allowed);
	}
	std::ofstream	ofs(loc.to());

	if (ofs.bad()) {
		_elog.log(LogLevel::error, "Couldn't open file at ", loc.to(), ".");
		return (http::Status::internal_error);
	}
	ofs << req.body();
	return (http::Status::created);
}

http::Status
Server::delete_(std::string&, http::Request const& req) {
	route::Location const	loc = locate(req.uri().path());

	if (!loc.allows_method(req.method())) {
		_elog.log(LogLevel::error,
			"Method ", http::to_string(req.method()),
			" not allowed at ", loc.to(), ".");
		return (http::Status::method_not_allowed);
	}
	if (!std::filesystem::remove(loc.to()))
		return (http::Status::internal_error);
	return (http::Status::no_content);
}

static http::Status
_get_file(std::string& body, route::Path const& path) {
	std::ifstream		file(path);
	std::stringstream	ss;

	if (!file.is_open())
		return (http::Status::forbidden);
	ss << file.rdbuf();
	body = ss.str();
	return (http::Status::ok);
}

static http::Status
_get_directory(std::string& body, route::Location const& loc) {
	if (loc.forbids_directory())
		return (http::Status::forbidden);
	if (loc.lists_directory())
		return (_list_directory(body, loc.to()));
	return (_get_file(body, loc.to() / loc.directory_file()));
}

static http::Status
_list_directory(std::string& body, route::Path const& path) {
	std::ostringstream	oss;

	for (auto const& entry: std::filesystem::directory_iterator(path))
		oss << entry.path() << ' ';
	body = oss.str();
	return (http::Status::ok);
}
