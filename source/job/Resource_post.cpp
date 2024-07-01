#include "job/Resource.hpp"

using job::Resource;

http::Status
Resource::_post(route::Location const& loc) {
	stdfs::path const&	pt = loc.to();

	_open_file(pt, std::ios::out | std::ios::trunc);
	if (_file.bad()) {
		_status = Status::failure; //  move this inside _open_file
		return (http::Status::internal_error);
	}
	return (http::Status::created);
}