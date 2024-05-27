#include "Route.hpp"

#include <algorithm>
#include <iostream>

static std::string	_relative_path(PathSegment, PathSegment);

// Modifiers

void
Route::extend(Path const& path) {
	if (path.root_path() != _fname)
		throw (std::invalid_argument("different root path"));
	_extend_core(++path.begin(), path.end());
}

void
Route::_extend_core(PathSegment seg, PathSegment end) {
	if (seg == end || seg->string().size() == 0) // route already contains this path
		return;

	auto const	subroute = std::find_if(_subroutes.begin(), _subroutes.end(),
		[seg](Route const& route) {
			return (route.filename() == seg->string());
		}
	);

	if (subroute == _subroutes.end()) // extend diverges here
		_subroutes.push_back(Route(*this, seg, end));
	else
		subroute->_extend_core(++seg, end);
}

// Accessors

RouteConfig
Route::follow(Path const& path) {
	if (path.root_path() != _fname)
		throw (std::invalid_argument("different root path"));
	return (_follow_core(++path.begin(), path.end()));
}

RouteConfig
Route::_follow_core(PathSegment seg, PathSegment end) {
	if (seg == end)
		return (RouteConfig(*this, _relative_path(seg, end)));
	auto const	subroute = std::find_if(_subroutes.begin(), _subroutes.end(),
		[seg](Route const& route) {
			return (route.filename() == seg->string());
		}
	);

	if (subroute == _subroutes.end())
		return (RouteConfig(*this, _relative_path(seg, end)));
	return (subroute->_follow_core(++seg, end));
}

static std::string
_relative_path(PathSegment seg, PathSegment end) {
	Path	path;

	while (seg != end)
		path /= *seg++;
	return (path.string());
}
