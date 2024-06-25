#include "Environment.hpp"

#include <cstring>
#include <sstream>

// Static methods

std::string
Environment::env_string(std::string const& key, std::string const& value) {
	return (key + "=" + value);
}

std::string
Environment::env_string(http::Header const& hdr) {
	std::ostringstream	oss;

	oss << "HTTP_";
	for (auto const& c: hdr.first) {
		if (c == '-')
			oss << '_';
		else
			oss << std::toupper(c);
	}
	oss << '=' << hdr.second;
	return (oss.str());
}

// Accessors

Environment::Container const&
Environment::vec() const noexcept {
	return (_ctr);
}

char const* const*
Environment::cenv() const noexcept {
	return (_cenv);
}

char**
Environment::cenv() noexcept {
	return (_cenv);
}

// Modifiers

void
Environment::append(http::Header const& hdr) {
	_ctr.push_back(env_string(hdr));
}

void
Environment::append(std::string const& key, std::string const& value) {
	_ctr.push_back(env_string(key, value));
}