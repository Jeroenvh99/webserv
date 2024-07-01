#include "job/Resource.hpp"

using job::Resource;

job::StatusOption
Resource::_post(route::Location const& loc) {
	stdfs::path const&	pt = loc.to();

	_open_file(pt, std::ios::out | std::ios::trunc);
	if (_file.bad())
		return (http::Status::internal_error);
	return (http::Status::created);
}