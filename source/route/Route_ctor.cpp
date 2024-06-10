#include "route.hpp"

using route::Route;
using route::Path;

Path const	Route::no_redirection = "";

Route::Route(Path const& path):
	BaseRoute(),
	_super(nullptr),
	_subroutes(),
	_fname(*(path.begin())) {
	auto const	next = ++path.begin();
	auto const	end = path.end();

	if (next != end && next->string().size() > 0)
		_subroutes.push_front(Route(*this, next, path.end()));
}

Route::Route(Route const& super, std::string const& fname):
	BaseRoute(true),
	_super(&super),
	_subroutes(),
	_fname(fname),
	_redirection(no_redirection) {}

Route::Route(Route const& super, std::string&& fname):
	BaseRoute(true),
	_super(&super),
	_subroutes(),
	_fname(fname),
	_redirection(no_redirection) {}

Route::Route(Route const& super, Path::iterator seg, Path::iterator end):
	BaseRoute(true),
	_super(&super),
	_subroutes(),
	_fname(*seg),
	_redirection(no_redirection) {
	if (++seg != end)
		_subroutes.push_front(Route(*this, seg, end));
}
