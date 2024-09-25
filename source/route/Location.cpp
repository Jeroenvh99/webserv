#include "route.hpp"

#include <stdexcept>
#include <iterator>
#include <string_view>

using route::Location;
using route::Route;

using PathIt = stdfs::path::iterator;

static std::string_view	_get_ext(std::string const&);
static stdfs::path&&	_path_append(stdfs::path&&, PathIt, PathIt);
static std::string		_to_string(PathIt, PathIt);

// Public constructors

Location::Location(Route const& rt, stdfs::path const& branch):
	Location(rt, branch.begin(), branch.end()) {}

Location::Location(Route const& rt, PathIt begin, PathIt end):
	Location(rt, begin, _find_cgi_end(begin, end, rt), end) {}

// Location::Location(Route const& rt, stdfs::path&&, stdfs::path&&, bool, std::string&&);

// Private constructors

Location::Location(Route const& rt, PathIt begin, IsCGI is_cgi, PathIt end):
	BaseRoute(rt),
	_from(_path_append(rt.from(), begin, end)),
	_to(_path_append(rt.to(), begin, is_cgi.second)),
	_is_cgi(is_cgi.first),
	_path_info(_to_string(is_cgi.second, end)) {}

// Public accessors

stdfs::path const&
Location::from() const noexcept {
	return (_from);
}

stdfs::path const&
Location::to() const noexcept {
	return (_to);
}

bool
Location::is_cgi() const noexcept {
	return (_is_cgi);
}

std::string const&
Location::path_info() const noexcept {
	return (_path_info);
}

Location
Location::translate(Route const& rt) const {
	return (rt.follow(_path_info));
}

// Private methods

Location::IsCGI
Location::_find_cgi_end(PathIt it, PathIt end, Route const& rt) {
	for (; it != end; ++it)
		if (rt.allows_cgi(_get_ext(*it)))
			return (std::make_pair(true, std::next(it, 1)));
	return (std::make_pair(false, end));
}

// Non-member helpers

static stdfs::path&&
_path_append(stdfs::path&& root, PathIt begin, PathIt end) {
	while (begin != end)
		root /= *begin++;
	return (std::move(root));
}

static std::string
_to_string(PathIt begin, PathIt end) {
	std::ostringstream	oss;

	while (begin != end)
		oss << stdfs::path::preferred_separator << (begin++)->string();
	return (oss.str());
}

static std::string_view
_get_ext(std::string const& fname) {
	if (fname.length() == 0)
		return (std::string_view(fname));
	auto	begin = std::next(fname.begin(), 1);

	while (begin != fname.end()) {
		if (*begin == '.') {
			auto	end = std::next(begin, 1);

			while (end != fname.end() && *end != '.')
				++end;
			if (end == fname.end())
				return (std::string_view(std::next(begin, 1), end)); // C++20
			begin = end;
		}
		else
			++begin;
	}
	return (std::string_view(begin, fname.end())); // C++20
}
