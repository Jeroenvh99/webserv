#include "route.hpp"

#include <algorithm>
#include <iostream>

using route::Route;
using route::Location;
using route::Path;

static bool			_is_final(Path::iterator, Path::iterator);
static std::string	_relative_path(Path::iterator, Path::iterator);

// Public accessors

std::string const&
Route::filename() const noexcept {
	return (_fname);
}

Path
Route::from() const {
	if (_super)
		return (_super->from() / _fname);
	return (_fname);
}

Path
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
	return (BaseRoute::forbids_directory);
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
Route::allows_cgi(std::string const& ext) const noexcept {
	if (_super && _cgiopt == CGIOption::inherits)
		return (_super->allows_cgi(ext));
	return (BaseRoute::allows_cgi(ext));
}

Location
Route::follow(Path const& path) const {
	if (path.root_path() != _fname)
		throw (std::invalid_argument("different root path"));
	return (_follow_core(++path.begin(), path.end()));
}

Location
Route::_follow_core(Path::iterator seg, Path::iterator end) const {
	if (_is_final(seg, end))
		return (Location(*this, _relative_path(seg, end)));

	auto	subroute = _subroute(seg->string());

	if (subroute == _subroutes.end())
		return (Location(*this, _relative_path(seg, end)));
	return (subroute->_follow_core(++seg, end));
}

Route&
Route::seek(Path const& path) {
	if (path.root_path() != _fname)
		throw (std::invalid_argument("different root path"));
	return (_seek_core(++path.begin(), path.end()));
}

Route&
Route::_seek_core(Path::iterator seg, Path::iterator end) {
	if (_is_final(seg, end))
		return (*this);

	auto	subroute = _subroute(seg->string());

	if (subroute == _subroutes.end())
		throw (std::invalid_argument("route not found"));
	return (subroute->_seek_core(++seg, end));
}

// Public modifiers

Route&
Route::extend(Path const& path) {
	if (path.root_path() != _fname)
		throw (std::invalid_argument("different root path"));
	return (_extend_core(++path.begin(), path.end()));
}

Route&
Route::_extend_core(Path::iterator seg, Path::iterator end) {
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

BaseRoute::MethodBitmask
Route::_super_allowed_methods() const noexcept {
	if (_super && _methopt == MethodOption::inherits)
		return (_super->_super_bitmask());
	return (_allowed_methods);
}

BaseRoute::DirectoryOption
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

BaseRoute::CGIOption
Route::_super_cgiopt() const noexcept {
	if (_super && _cgiopt == CGIOption::inherits)
		return (_super->_super_cgiopt());
	return (_cgiopt);
}

BaseRoute::ExtensionSet
Route::_super_cgi() const noexcept {
	if (_super && _cgiopt == CGIOption::inherits)
		return (_super->_super_cgi());
	return (_cgi);
}

// Non-member helpers

static bool
_is_final(Path::iterator seg, Path::iterator end) {
	return (seg == end || seg->string().size() == 0);
}

static std::string
_relative_path(Path::iterator seg, Path::iterator end) {
	Path	path;

	while (seg != end)
		path /= *seg++;
	return (path.string());
}
