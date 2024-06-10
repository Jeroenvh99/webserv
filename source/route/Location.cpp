#include "route.hpp"

using route::Location;
using route::Path;

Location::Location(Route const& rt, Path const& relative_path):
	BaseRoute(rt),
	_from(rt.from() / relative_path),
	_to(rt.to() / relative_path),
	_path_info() {}

std::filesystem::path const&
Location::from() const noexcept {
	return (_from);
}

std::filesystem::path const&
Location::to() const noexcept {
	return (_to);
}

std::string const&
Location::path_info() const noexcept {
	return (_path_info);
}
