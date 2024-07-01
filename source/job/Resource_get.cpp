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
	_open_file(pt, std::ios::in);

	if (!_file.is_open())
		return (http::Status::not_found);
	return (http::Status::ok);
}

http::Status
Resource::_get_directory(route::Location const& loc) {
	if (loc.forbids_directory())
		return (http::Status::forbidden);
	if (loc.lists_directory())
		return (_get_directory_list(loc.to()));
	return (_get_file(loc.to() / loc.directory_file()));
}

http::Status
Resource::_get_directory_list(stdfs::path const& path) {
	std::stringstream	ss;

	for (auto const& entry: std::filesystem::directory_iterator(path))
		ss << entry.path() << ' ';
	_open_builtin(std::move(ss));
	return (http::Status::ok);
}