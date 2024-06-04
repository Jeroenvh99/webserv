#include "URI.hpp"

#include <algorithm>
#include <sstream>

using Cit = std::string::const_iterator;

static Cit			_delim_path(std::string const&);
static Cit			_delim_query(std::string const&);
static std::string	_make_arg(Cit, Cit);

// Basic operations

URI::URI(std::string const& str):
	URI(str.begin(), _delim_path(str), _delim_query(str), str.end()) {}

URI::URI(Cit begin, Cit path_end, Cit query_end, Cit end):
	_path(begin, path_end),
	_query(_make_arg(path_end, query_end)),
	_fragment(_make_arg(query_end, end)) {}

// Conversions

URI::operator std::string() const {
	std::ostringstream	oss(_path.string());

	if (_query.size() > 0)
		oss << query_sign << _query;
	if (_fragment.size() > 0)
		oss << fragment_sign << _fragment;
	return (oss.str());
}

// Accessor methods

std::filesystem::path const&
URI::path() const noexcept {
	return (_path);
}

std::string const&
URI::query() const noexcept {
	return (_query);
}

std::string const&
URI::fragment() const noexcept {
	return (_fragment);
}

// Non-member helpers

static Cit
_delim_path(std::string const& str) {
	return (std::find(str.begin(), str.end(), URI::query_sign));
}

static Cit
_delim_query(std::string const& str) {
	return (std::find(str.begin(), str.end(), URI::fragment_sign));
}

static std::string
_make_arg(Cit begin, Cit end) {
	if (begin++ != end)
		return (std::string(begin, end));
	return ("");
}
