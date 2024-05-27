#include "Route.hpp"

#include <algorithm>
#include <iostream>

static Route::SubrouteIt	_find_subroute(Route::SubrouteCtr&, std::string const&);
static bool					_is_final(PathSegment, PathSegment);
static std::string			_relative_path(PathSegment, PathSegment);

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

	SubrouteIt	subroute = _find_subroute(_subroutes, seg->string());

	if (subroute == _subroutes.end()) { // extend diverges here
		_subroutes.push_front(Route(*this, std::move(*seg)));
		return (_subroutes.front()._extend_core(++seg, end));
	}
	else
		return (subroute->_extend_core(++seg, end));
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
	if (_is_final(seg, end))
		return (RouteConfig(*this, _relative_path(seg, end)));

	SubrouteIt	subroute = _find_subroute(_subroutes, seg->string());

	if (subroute == _subroutes.end())
		return (RouteConfig(*this, _relative_path(seg, end)));
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

	SubrouteIt	subroute = _find_subroute(_subroutes, seg->string());

	if (subroute == _subroutes.end())
		throw (std::invalid_argument("route not found"));
	return (subroute->_seek_core(++seg, end));
}

static bool
_is_final(PathSegment seg, PathSegment end) {
	return (seg == end || seg->string().size() == 0);
}

static Route::SubrouteIt
_find_subroute(Route::SubrouteCtr& ctr, std::string const& fname) {
	return (std::find_if(ctr.begin(), ctr.end(),
		[fname](Route const& route) { return (route.filename() == fname); } ));
}

static std::string
_relative_path(PathSegment seg, PathSegment end) {
	Path	path;

	while (seg != end)
		path /= *seg++;
	return (path.string());
}
