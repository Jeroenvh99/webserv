#include "job/resource.hpp"

using job::Resource;

static std::string	make_headers_and_body(route::Location const&);

http::Status
Resource::_post(route::Location const& loc) {
	stdfs::path const&	pt = loc.to();

	_open_ofile(pt);
	if (!_ofs.good())
		return (http::Status::internal_error);
	_iss.clear();
	_iss.str(make_headers_and_body(loc));

	return (http::Status::created);
}

static std::string
make_headers_and_body(route::Location const& loc) {
	std::ostringstream	oss;
	std::string			representation = loc.from().c_str();

	oss << "Connection: keep-alive\r\n"
		<< "Content-Type: text/plain\r\n"
		<< "Content-Length: " << representation.length() << "\r\n"
		<< "Location: " << representation << "\r\n\r\n"
		<< representation;
	return (oss.str());
}