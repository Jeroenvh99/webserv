#include "URI.hpp"

#include <algorithm>

using Cit = std::string::const_iterator;

static Cit	_delim_path(std::string const&);
static Cit	_delim_query(std::string const&);

// Basic operations

URI::URI(std::string const& str):
	URI(str.begin(), _delim_path(str), _delim_query(str), str.end()) {}

URI::URI(Cit begin, Cit path_end, Cit query_end, Cit end):
	_path(begin, path_end++),
	_query(path_end, query_end++),
	_fragment(query_end, end) {}

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
	return (std::find(str.begin(), str.end(), '?'));
}

static Cit
_delim_query(std::string const& str) {
	return (std::find(str.begin(), str.end(), '#'));
}
