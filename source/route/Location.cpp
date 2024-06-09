#include "route.hpp"

using route::Location;
using route::Path;

Location::Location(Route const& rt):
	_methopt(MethodOption::own),
	_allowed_methods(rt._super_allowed_methods()),
	_diropt(rt._super_diropt()),
	_directory_file(rt._super_directory_file()),
	_cgiopt(rt._super_cgiopt()),
	_cgi(rt._super_cgi()),
	_from(rt.from()),
	_to(rt.to()),
	_path_info()) {}

std::filesystem::path const&
Location::from() const noexcept {
	return (_from);
}

std::filesystem::path const&
Location::to() const noexcept {
	return (_to);
}

std::string const&
Location::path_info() const noexcept {
	return (_path_info);
}
