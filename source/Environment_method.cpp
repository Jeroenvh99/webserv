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

// Public accessors

Environment::Container const&
Environment::ctr() const noexcept {
	return (_ctr);
}

// Public modifiers

void
Environment::append(http::Header const& hdr) {
	_ctr.push_back(env_string(hdr));
}

void
Environment::append(std::string const& key, std::string const& value) {
	_ctr.push_back(env_string(key, value));
}

// Conversions

char**
Environment::make_cenv() {
	size_t const	total_size = _cenv_size + static_size + _ctr.size();
	char**			p = new char*[total_size + 1];
	char** const&	cenv_offset = p + _cenv_size;
	char** const&	static_offset = cenv_offset + static_size;

	::memmove(p, _cenv, _cenv_size * sizeof(char*));
	::memmove(cenv_offset, static_env.data(), static_size * sizeof(char*));
	for (size_t i = 0; i < _ctr.size(); ++i)
		static_offset[i] = _ctr.at(i).data();
	p[total_size] = nullptr;
	return (p);
}
