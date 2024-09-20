#include "route.hpp"

using route::Route;
using PathIt = stdfs::path::iterator;

// Static variable declarations

stdfs::path const	Route::no_redirection = "";

// Public constructors

Route::Route(stdfs::path const& path):
	BaseRoute(),
	_super(nullptr),
	_subroutes(),
	_fname(*(path.begin())) {
	extend(path);
}

Route::Route(Route &&route):
	BaseRoute(route),
	_super(route._super),
	_subroutes(std::move(route._subroutes)),
	_fname(std::move(route._fname)),
	_redirection(route._redirection) {
		for (auto &subroute : _subroutes) {
			subroute._super = this;
		}
		route._super = nullptr;
	}

// Private constructors

Route::Route(Route const& super, PathIt seg, PathIt end):
	BaseRoute(true),
	_super(&super),
	_subroutes(),
	_fname(*seg),
	_redirection(no_redirection) {
	if (++seg != end)
		_subroutes.push_front(Route(*this, seg, end));
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
