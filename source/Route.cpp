#include "Route.hpp"

#include "network/utils.tpp"

static std::string	_get_path(std::string const&, std::string const&);

// Basic operations

Route::Route(std::string const& to, std::string const& dfl,
		std::initializer_list<http::Method> methods):
	_to(to),
	_dfl(dfl),
	_methods(network::_get_bitmask(methods)) {}

Route::Route(Route const& templ, std::string const& path):
	_to(_get_path(templ._to, path)),
	_dfl(templ._dfl),
	_methods(templ._methods) {}

// Accessors

std::string const&
Route::to() const noexcept {
	return (_to);
}

std::string const&
Route::default_file() const noexcept {
	return (_dfl);
}

bool
Route::allows_method(http::Method method) const noexcept {
	return (_methods & static_cast<Bitmask>(method));
}

// Non-member helpers

static std::string
_get_path(std::string const& templ, std::string const& spec) {
	return (templ + spec); // DB: this should be greatly expanded
}
