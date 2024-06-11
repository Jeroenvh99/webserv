#include "route.hpp"

#include <algorithm>
#include <iostream>

using route::Route;
using route::Location;

static bool	_is_final(stdfs::path::iterator, stdfs::path::iterator);

// Public accessors

std::string const&
Route::filename() const noexcept {
	return (_fname);
}

stdfs::path
Route::from() const {
	if (_super)
		return (_super->from() / _fname);
	return (_fname);
}

stdfs::path
Route::to() const {
	if (_redirection == no_redirection) {
		if (_super)
			return (_super->to() / _fname);
		return (from());
	}
	return (_redirection);
}

bool
Route::lists_directory() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->lists_directory());
	return (BaseRoute::lists_directory());
}

bool
Route::forbids_directory() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->forbids_directory());
	return (BaseRoute::forbids_directory());
}

std::string const&
Route::directory_file() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->directory_file());
	return (BaseRoute::directory_file());
}

bool
Route::allows_method(http::Method method) const noexcept {
	if (_super && _methopt == MethodOption::inherits)
		return (_super->allows_method(method));
	return (BaseRoute::allows_method(method));
}

bool
Route::allows_cgi(std::string_view const& ext) const noexcept {
	if (_super && _cgiopt == CGIOption::inherits)
		return (_super->allows_cgi(ext));
	return (BaseRoute::allows_cgi(ext));
}

bool
Route::allows_cgi(std::string const& ext) const noexcept {
	return (allows_cgi(std::string_view(ext)));
}

Location
Route::follow(stdfs::path const& path) const {
	if (path.root_path() != _fname)
		throw (std::invalid_argument("different root path"));
	return (_follow_core(++path.begin(), path.end()));
}

Location
Route::_follow_core(stdfs::path::iterator seg, stdfs::path::iterator end) const {
	if (_is_final(seg, end))
		return (Location(*this));

	auto	subroute = _subroute(seg->string());

	if (subroute == _subroutes.end())
		return (Location(*this, seg, end));
	return (subroute->_follow_core(++seg, end));
}

Route&
Route::seek(stdfs::path const& path) {
	if (path.root_path() != _fname)
		throw (std::invalid_argument("different root path"));
	return (_seek_core(++path.begin(), path.end()));
}

Route&
Route::_seek_core(stdfs::path::iterator seg, stdfs::path::iterator end) {
	if (_is_final(seg, end))
		return (*this);

	auto	subroute = _subroute(seg->string());

	if (subroute == _subroutes.end())
		throw (std::invalid_argument("route not found"));
	return (subroute->_seek_core(++seg, end));
}

// Public modifiers

Route&
Route::extend(stdfs::path const& path) {
	if (path.root_path() != _fname)
		throw (std::invalid_argument("different root path"));
	return (_extend_core(++path.begin(), path.end()));
}

Route&
Route::_extend_core(stdfs::path::iterator seg, stdfs::path::iterator end) {
	if (_is_final(seg, end)) // route already contains this path
		return (*this);

	auto	subroute = _subroute(seg->string());

	if (subroute == _subroutes.end()) { // extend diverges here
		_subroutes.push_front(Route(*this, std::move(*seg)));
		return (_subroutes.front()._extend_core(++seg, end));
	}
	else
		return (subroute->_extend_core(++seg, end));
}

Route&
Route::redirect(stdfs::path const& path) {
	_redirection = path;
	return (*this);
}

Route&
Route::list_directory() noexcept {
	_directory_file.clear();
	_diropt = DirectoryOption::listing;
	return (*this);
}

Route&
Route::forbid_directory() noexcept {
	_directory_file.clear();
	_diropt = DirectoryOption::forbid;
	return (*this);
}

Route&
Route::set_directory_file(std::string const& fname) {
	_diropt = DirectoryOption::default_file;
	_directory_file = fname;
	return (*this);
}

Route&
Route::reset_diropts() noexcept {
	_directory_file.clear();
	if (_super)
		_diropt = DirectoryOption::inherits;
	else
		_diropt = DirectoryOption::forbid;
	return (*this);
}

Route&
Route::allow_method(http::Method method) noexcept {
	_methopt = MethodOption::own;
	_allowed_methods |= static_cast<MethodBitmask>(method);
	return (*this);
}

Route&
Route::disallow_method(http::Method method) noexcept {
	_methopt = MethodOption::own;
	_allowed_methods &= ~(static_cast<MethodBitmask>(method));
	return (*this);
}

Route&
Route::reset_methods() noexcept {
	_allowed_methods = 0;
	if (_super)
		_methopt = MethodOption::inherits;
	return (*this);
}

Route&
Route::allow_cgi(std::string const& ext) {
	_cgiopt = CGIOption::allow;
	_cgi.insert(ext);
	return (*this);
}

Route&
Route::disallow_cgi(std::string const& ext) {
	auto const	it = _cgi.find(ext);

	if (it != _cgi.end())
		_cgi.erase(it);
	return (*this);
}

Route&
Route::reset_cgi() {
	_cgi.clear();
	if (_super)
		_cgiopt = CGIOption::inherits;
	else
		_cgiopt = CGIOption::disallow;
	return (*this);
}

// Private accessors

Route::Container::iterator
Route::_subroute(std::string const& fname) noexcept {
	return (std::find_if(_subroutes.begin(), _subroutes.end(),
		[fname](Route const& route) { return (route.filename() == fname); } ));
}

Route::Container::const_iterator
Route::_subroute(std::string const& fname) const noexcept {
	return (std::find_if(_subroutes.begin(), _subroutes.end(),
		[fname](Route const& route) { return (route.filename() == fname); } ));
}

Route::MethodBitmask
Route::_super_allowed_methods() const noexcept {
	if (_super && _methopt == MethodOption::inherits)
		return (_super->_super_allowed_methods());
	return (_allowed_methods);
}

Route::DirectoryOption
Route::_super_diropt() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->_super_diropt());
	return (_diropt);
}

std::string const&
Route::_super_directory_file() const noexcept {
	if (_super && _diropt == DirectoryOption::inherits)
		return (_super->_super_directory_file());
	return (_directory_file);
}

Route::CGIOption
Route::_super_cgiopt() const noexcept {
	if (_super && _cgiopt == CGIOption::inherits)
		return (_super->_super_cgiopt());
	return (_cgiopt);
}

Route::ExtensionSet const&
Route::_super_cgi() const noexcept {
	if (_super && _cgiopt == CGIOption::inherits)
		return (_super->_super_cgi());
	return (_cgi);
}

// Non-member helpers

static bool
_is_final(stdfs::path::iterator seg, stdfs::path::iterator end) {
	return (seg == end || seg->string().size() == 0);
}
