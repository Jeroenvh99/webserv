#include "route.hpp"

using route::Config;
using route::Path;
using route::PathSegment;

// Basic operations

Path const	Config::no_redirection = "";

Config::Config(std::string&& fname):
	_super(nullptr),
	_fname(fname),
	_redirection(no_redirection),
	_methopt(MethodOption::own), _allowed_methods(0),
	_diropt(DirectoryOption::forbid), _directory_file(),
	_cgiopt(CGIOption::disallow), _cgi() {}

Config::Config(Config const& super, std::string const& fname):
	_super(&super),
	_fname(fname),
	_redirection(no_redirection),
	_methopt(MethodOption::inherits), _allowed_methods(0),
	_diropt(DirectoryOption::inherits), _directory_file(),
	_cgiopt(CGIOption::inherits), _cgi() {}

Config::Config(Config const& super, std::string&& fname):
	_super(&super),
	_fname(fname),
	_redirection(no_redirection),
	_methopt(MethodOption::inherits), _allowed_methods(0),
	_diropt(DirectoryOption::inherits), _directory_file(),
	_cgiopt(CGIOption::inherits), _cgi() {}

// Accessor methods

std::string const&
Config::filename() const noexcept {
	return (_fname);
}

Path
Config::from() const {
	if (_super)
		return (_super->from() / _fname);
	return (_fname);
}

Path
Config::to() const {
	if (_redirection == no_redirection) {
		if (_super)
			return (_super->to() / _fname);
		return (from());
	}
	return (_redirection);
}

bool
Config::lists_directory() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->lists_directory());
	return (_diropt == DirectoryOption::listing);
}

bool
Config::forbids_directory() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->forbids_directory());
	return (_diropt == DirectoryOption::forbid);
}

std::string const&
Config::directory_file() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->directory_file());
	return (_directory_file);
}

bool
Config::allows_method(http::Method method) const noexcept {
	if (_super && _methopt == MethodOption::inherits)
		return (_super->allows_method(method));
	return (_allowed_methods & static_cast<MethodBitmask>(method));
}

bool
Config::allows_cgi(std::string const& ext) const noexcept {
	if (_super && _cgiopt == CGIOption::inherits)
		return (_super->allows_cgi(ext));
	return (_cgiopt == CGIOption::allow
		&& _cgi.find(ext) != _cgi.end());
}

// Modifier methods

Config&
Config::redirect(Path const& path) {
	_redirection = path;
	return (*this);
}

Config&
Config::list_directory() noexcept {
	_directory_file.clear();
	_diropt = DirectoryOption::listing;
	return (*this);
}

Config&
Config::forbid_directory() noexcept {
	_directory_file.clear();
	_diropt = DirectoryOption::forbid;
	return (*this);
}

Config&
Config::set_directory_file(std::string const& fname) {
	_diropt = DirectoryOption::default_file;
	_directory_file = fname;
	return (*this);
}

Config&
Config::reset_diropts() noexcept {
	_directory_file.clear();
	if (_super)
		_diropt = DirectoryOption::inherits;
	else
		_diropt = DirectoryOption::forbid;
	return (*this);
}

Config&
Config::allow_method(http::Method method) noexcept {
	_methopt = MethodOption::own;
	_allowed_methods |= static_cast<MethodBitmask>(method);
	return (*this);
}

Config&
Config::disallow_method(http::Method method) noexcept {
	_methopt = MethodOption::own;
	_allowed_methods &= ~(static_cast<MethodBitmask>(method));
	return (*this);
}

Config&
Config::reset_methods() noexcept {
	_allowed_methods = 0;
	if (_super)
		_methopt = MethodOption::inherits;
	return (*this);
}

Config&
Config::allow_cgi(std::string const& ext) {
	_cgiopt = CGIOption::allow;
	_cgi.insert(ext);
	return (*this);
}

Config&
Config::disallow_cgi(std::string const& ext) {
	auto const	it = _cgi.find(ext);

	if (it != _cgi.end())
		_cgi.erase(it);
	return (*this);
}

Config&
Config::reset_cgi() {
	_cgi.clear();
	if (_super)
		_cgiopt = CGIOption::inherits;
	else
		_cgiopt = CGIOption::disallow;
	return (*this);
}
