#include "Route.hpp"

Route::Route(Path const& path):
	RouteConfig(*(path.begin())),
	_subroutes() {
	auto const	next = path.begin()++;
	auto const	end = path.end();

	if (next != end && next->string().size() > 0)
		_subroutes.push_back(Route(*this, next, path.end()));
}

Route::Route(Route const& super, std::string const& fname):
	RouteConfig(super, fname),
	_subroutes() {}

Route::Route(Route const& super, PathSegment seg, PathSegment end):
	RouteConfig(super, *seg),
	_subroutes() {
	if (++seg != end)
		_subroutes.push_back(Route(*this, seg, end));
}
