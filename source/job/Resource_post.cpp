#include "job/Resource.hpp"

using job::Resource;

http::Status
Resource::_post(route::Location const& loc) {
	stdfs::path const&	pt = loc.to();

	_open_ofile(pt);
	if (!_ofs.good())
		return (http::Status::internal_error);
	_iss.clear();
	_iss.str(make_headers_post(loc));
	_iss.str("there is something new on you server\r\n");

	return (http::Status::created);
}