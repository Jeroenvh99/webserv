#include "route.hpp"

#include <iterator>
#include <string_view>

using route::Location;
using route::Route;
using route::Path;

using PathIt = std::filesystem::path::iterator;

static PathIt					_find_path_end(std::filesystem::path const&, Route const&);
static std::string_view			_get_ext(std::string const&);
static std::filesystem::path	_path_append(std::filesystem::path const&, PathIt, PathIt);
static std::string				_get_path_info(PathIt, PathIt);

Location::Location(Route const& rt, Path const& branch):
	Location(rt, branch.begin(), _find_path_end(branch, rt), branch.end()) {}

Location::Location(Route const& rt, PathIt begin, PathIt path_end, PathIt end):
	BaseRoute(rt),
	_from(_path_append(rt.from(), begin, path_end)),
	_to(_path_append(rt.to(), begin, path_end)),
	_path_info(_get_path_info(path_end, end)) {}

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

// Non-member helpers

static std::filesystem::path
_path_append(std::filesystem::path const& root, PathIt begin, PathIt end) {
	std::filesystem::path	pt(root);

	while (begin != end)
		pt /= *begin++;
	return (pt);
}

static std::string
_get_path_info(PathIt begin, PathIt end) {
	std::ostringstream	oss;

	while (begin != end)
		oss << "/" << (begin++)->string();
	return (oss.str());
}

static PathIt
_find_path_end(std::filesystem::path const& pt, Route const& rt) {
	for (auto seg = pt.begin(); seg != pt.end(); ++seg)
		if (rt.allows_cgi(_get_ext(*seg)))
			return (std::next(seg, 1));
	return (pt.end());
}

static std::string_view
_get_ext(std::string const& fname) {
	auto	begin = std::next(fname.begin(), 1);

	while (begin != fname.end()) {
		if (*begin == '.') {
			auto	end = std::next(begin, 1);

			while (end != fname.end() && *end != '.')
				++end;
			if (end == fname.end())
				return (std::string_view(std::next(begin, 1), end));
			begin = end;
		}
		else
			++begin;
	}
	return (std::string_view(begin, fname.end()));
}
