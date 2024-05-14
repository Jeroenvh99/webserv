#include "Route.hpp"

#include "network/utils.tpp"

Route::Route(std::string const& to, std::string const& dfl,
		std::initializer_list<http::Method> methods):
	_to(to),
	_dfl(dfl),
	_methods(network::_get_bitmask(methods)) {}

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
