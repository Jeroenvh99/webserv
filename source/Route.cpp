#include "Route.hpp"

Route::Route(std::string const& to, std::string const& dfl,
		http::RequestMethod methods):
	_to(to),
	_dfl(dfl),
	_methods(methods) {}

std::string const&
Route::to() const noexcept {
	return (_to);
}

std::string const&
Route::default_file() const noexcept {
	return (_dfl);
}

bool
Route::allows_method(http::RequestMethod method) const noexcept {
	return (_methods & method);
}
