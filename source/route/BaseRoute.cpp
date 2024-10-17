#include "route.hpp"

using route::BaseRoute;

// Basic operations

BaseRoute::BaseRoute(bool inherit):
	_methopt(inherit ? MethodOption::inherits : MethodOption::own),
	_allowed_methods(0),
	_diropt(inherit ? DirectoryOption::inherits : DirectoryOption::forbid),
	_directory_file(),
	_cgiopt(inherit ? CGIOption::inherits : CGIOption::disallow),
	_cgi() {}

BaseRoute::BaseRoute(Route const& that):
	_methopt(MethodOption::own),
	_allowed_methods(that._super_allowed_methods()),
	_diropt(that._super_diropt()),
	_directory_file(that._super_directory_file()),
	_cgiopt(that._super_cgiopt()),
	_cgi(that._super_cgi()) {}

// Accessor methods

bool
BaseRoute::lists_directory() const noexcept {
	return (_diropt == DirectoryOption::listing);
}

bool
BaseRoute::forbids_directory() const noexcept {
	return (_diropt == DirectoryOption::forbid);
}

std::string const&
BaseRoute::directory_file() const noexcept {
	return (_directory_file);
}

bool
BaseRoute::allows_method(http::Method method) const noexcept {;
	return (_allowed_methods & static_cast<MethodBitmask>(method));
}

bool
BaseRoute::allows_cgi(std::string const& ext) const noexcept {
	return (allows_cgi(std::string_view(ext)));
}

bool
BaseRoute::allows_cgi(std::string_view const& ext) const noexcept {
	return (_cgi.find(ext) != _cgi.end());
}
