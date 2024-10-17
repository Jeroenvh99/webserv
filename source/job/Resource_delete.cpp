#include "job/resource.hpp"

using job::Resource;

static std::string	make_headers();

http::Status
Resource::_delete(route::Location const& loc) {
	stdfs::path const&	pt = loc.to();

	if (!std::filesystem::exists(pt))
		return (http::Status::not_found);
	if (!std::filesystem::remove(pt))
		return (http::Status::internal_error);
	_iss.clear();
	_iss.str(make_headers());
	return (http::Status::no_content);
}

static std::string
make_headers() {
	std::ostringstream	oss;

	oss << "Connection: keep-alive\r\n";
	oss << "\r\n";
	return (oss.str());
}