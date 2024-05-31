#include "route.hpp"

#include <algorithm>
#include <iostream>

using route::Route;
using route::Location;
using route::Path;
using route::PathSegment;

static bool			_is_final(PathSegment, PathSegment);
static std::string	_relative_path(PathSegment, PathSegment);

// Modifiers

Route&
Route::extend(Path const& path) {
	if (path.root_path() != _fname)
		throw (std::invalid_argument("different root path"));
	return (_extend_core(++path.begin(), path.end()));
}

Route&
Route::_extend_core(PathSegment seg, PathSegment end) {
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

// Accessors

Location
Route::follow(Path const& path) const {
	if (path.root_path() != _fname)
		throw (std::invalid_argument("different root path"));
	return (_follow_core(++path.begin(), path.end()));
}

Location
Route::_follow_core(PathSegment seg, PathSegment end) const {
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
Route::_seek_core(PathSegment seg, PathSegment end) {
	if (_is_final(seg, end))
		return (*this);

	auto	subroute = _subroute(seg->string());

	if (subroute == _subroutes.end())
		throw (std::invalid_argument("route not found"));
	return (subroute->_seek_core(++seg, end));
}

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

// Non-member helpers

static bool
_is_final(PathSegment seg, PathSegment end) {
	return (seg == end || seg->string().size() == 0);
}

static std::string
_relative_path(PathSegment seg, PathSegment end) {
	Path	path;

	while (seg != end)
		path /= *seg++;
	return (path.string());
}
