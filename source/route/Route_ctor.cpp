#include "route.hpp"

using route::Route;
using route::Path;

Route::Route(Path const& path):
	BaseRoute(*(path.begin())),
	_super(nullptr),
	_subroutes() {
	auto const	next = ++path.begin();
	auto const	end = path.end();

	if (next != end && next->string().size() > 0)
		_subroutes.push_front(Route(*this, next, path.end()));
}

Route::Route(Route const& super, std::string const& fname):
	BaseRoute(fname),
	_super(&super),
	_subroutes() {}

Route::Route(Route const& super, PathSegment seg, PathSegment end):
	BaseRoute(*seg),
	_super(&super),
	_subroutes() {
	if (++seg != end)
		_subroutes.push_front(Route(*this, seg, end));
}
