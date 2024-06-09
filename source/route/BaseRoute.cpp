#include "route.hpp"

using route::BaseRoute;
using route::Path;
using route::PathSegment;

// Basic operations

Path const	BaseRoute::no_redirection = "";

BaseRoute::BaseRoute(std::string&& fname):
	_fname(fname),
	_redirection(no_redirection),
	_methopt(MethodOption::own), _allowed_methods(0),
	_diropt(DirectoryOption::forbid), _directory_file(),
	_cgiopt(CGIOption::disallow), _cgi() {}

BaseRoute::BaseRoute(std::string const& fname):
	_fname(fname),
	_redirection(no_redirection),
	_methopt(MethodOption::inherits), _allowed_methods(0),
	_diropt(DirectoryOption::inherits), _directory_file(),
	_cgiopt(CGIOption::inherits), _cgi() {}

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
BaseRoute::allows_method(http::Method method) const noexcept {
	return (_allowed_methods & static_cast<MethodBitmask>(method));
}
