#include "job/Resource.hpp"

using job::Resource;

http::Status
Resource::_get(route::Location const& loc) {
	if (!std::filesystem::exists(loc.to()))
		return (http::Status::not_found);
	if (std::filesystem::is_directory(loc.to()))
		return (_get_directory(loc));
	return (_get_file(loc.to()));
}

http::Status
Resource::_get_file(stdfs::path const& pt) {
	_ifs.open(pt);

	if (!_ifs.is_open())
		return (http::Status::not_found);
	_iss.clear();
	_iss.str(make_headers_get(pt));
	return (http::Status::ok);
}

http::Status
Resource::_get_directory(route::Location const& loc) {
	if (loc.forbids_directory())
		return (http::Status::forbidden);
	if (loc.lists_directory())
		return (_open_builtin(_make_directory_list(loc.to())), http::Status::ok);
	return (_get_file(loc.to() / loc.directory_file()));
}
