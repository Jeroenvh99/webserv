#include "Route.hpp"

// Basic operations

Path const	RouteConfig::no_redirection = "";

RouteConfig::RouteConfig(std::string&& fname):
	_super(nullptr),
	_fname(fname),
	_redirection(no_redirection),
	_methopt(MethodOption::own), _allowed_methods(0),
	_diropt(DirectoryOption::forbid), _directory_file(),
	_cgiopt(CGIOption::disallow), _cgi() {}

RouteConfig::RouteConfig(RouteConfig const& super, std::string&& fname):
	_super(&super),
	_fname(fname),
	_redirection(no_redirection),
	_methopt(MethodOption::inherits), _allowed_methods(0),
	_diropt(DirectoryOption::inherits), _directory_file(),
	_cgiopt(CGIOption::inherits), _cgi() {}

// Accessor methods

std::string const&
RouteConfig::filename() const noexcept {
	return (_fname);
}

Path
RouteConfig::from() const {
	if (_super)
		return (_super->from() / _fname);
	return (_fname);
}

Path
RouteConfig::to() const {
	if (_super && _redirection != no_redirection)
		return (_super->to() / _fname);
	return (from());
}

bool
RouteConfig::lists_directory() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->lists_directory());
	return (_diropt == DirectoryOption::listing);
}

bool
RouteConfig::forbids_directory() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->lists_directory());
	return (_diropt == DirectoryOption::forbid);
}

std::string const&
RouteConfig::directory_file() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->directory_file());
	return (_directory_file);
}

bool
RouteConfig::allows_method(http::Method method) const noexcept {
	if (_super && _methopt == MethodOption::inherits)
		return (_super->allows_method(method));
	return (_allowed_methods & static_cast<MethodBitmask>(method));
}

bool
RouteConfig::allows_cgi(std::string const& ext) const noexcept {
	if (_super && _cgiopt == CGIOption::inherits)
		return (_super->allows_cgi(ext));
	return (_cgiopt == CGIOption::allow
		&& _cgi.find(ext) != _cgi.end());
}

// Modifier methods

void
RouteConfig::redirect(Path const& path) {
	_redirection = path;
}

void
RouteConfig::list_directory() noexcept {
	_directory_file.clear();
	_diropt = DirectoryOption::listing;
}

void
RouteConfig::forbid_directory() noexcept {
	_directory_file.clear();
	_diropt = DirectoryOption::forbid;
}

void
RouteConfig::set_directory_file(std::string const& fname) {
	_diropt = DirectoryOption::default_file;
	_directory_file = fname
}

void
RouteConfig::reset_diropts() noexcept {
	_directory_file.clear();
	if (_super)
		_diropt = DirectoryOption::inherits;
	else
		_diropt = DirectoryOption::forbid;
}

void
RouteConfig::allow_method(http::Method method) noexcept {
	_methopt = MethodOption::own;
	_allowed_methods |= static_cast<MethodBitmask>(method);
}

void
RouteConfig::disallow_method(http::Method method) noexcept {
	_methopt = MethodOption::own;
	_allowed_methods &= ~(static_cast<MethodBitmask>(method));
}

void
RouteConfig::reset_methods() noexcept {
	_allowed_methods = 0;
	if (_super)
		_methopt = MethodOption::inherits;
}

void
RouteConfig::allow_cgi(std::string const& ext) {
	_cgiopt = CGIOption::allow;
	_cgi.insert(ext);
}

void
RouteConfig::disallow_cgi(std::string const& ext) {
	auto const	it = _cgi.find(ext);

	if (it != _cgi.end())
		_cgi.erase(it);
}

void
RouteConfig::reset_cgi() {
	_cgi.clear();
	if (_super)
		_cgiopt = CGIOption::inherits;
	else
		_cgiopt = CGIOption::disallow;
}
