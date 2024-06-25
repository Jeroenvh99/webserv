#include "job/Resource.hpp"

using job::Resource;

StatusOption
Resource::_post(stdfs::path const& pt) {
	_open_file(pt, std::ios::out | std::ios::trunc);

	if (_file.bad())
		return (http::Status::internal_error);
	return (http::Status::created);
}