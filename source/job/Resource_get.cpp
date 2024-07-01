#include "job/Resource.hpp"

using job::Resource;

job::StatusOption
Resource::_get(route::Location const& loc) {
	if (!std::filesystem::exists(loc.to()))
		return (http::Status::not_found);
	if (std::filesystem::is_directory(loc.to()))
		return (_get_directory(loc));
	return (_get_file(loc.to()));
}

job::StatusOption
Resource::_get_file(stdfs::path const& pt) {
	_open_file(pt, std::ios::in);

	if (!_file.is_open())
		return (http::Status::not_found);
	return (std::nullopt);
}

job::StatusOption
Resource::_get_directory(route::Location const& loc) {
	if (loc.forbids_directory())
		return (http::Status::forbidden);
	if (loc.lists_directory())
		return (_get_directory_list(loc.to()));
	return (_get_file(loc.to() / loc.directory_file()));
}

job::StatusOption
Resource::_get_directory_list(stdfs::path const& path) {
	std::ostringstream	oss;

	for (auto const& entry: std::filesystem::directory_iterator(path))
		oss << entry.path() << ' ';
	_open_builtin(oss.str());
	return (http::Status::ok);
}