#include "Environment.hpp"

#include <cstring>
#include <sstream>

std::array<std::string, Environment::base_size> const	Environment::base_vars{
	"SERVER_SOFTWARE=Webserv/1.0",
	"GATEWAY_INTERFACE=CGI/1.1"};

char**	Environment::_cenv;
size_t	Environment::_cenv_size;

Environment::Environment(std::initializer_list<std::string> ilist):
	_local(ilist) {}

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

void
Environment::append(http::Header const& hdr) {
	_local.push_back(env_string(hdr));
}

void
Environment::append(std::string const& key, std::string const& value) {
	_local.push_back(env_string(key, value));
}

char**
Environment::make_cenv() {
	size_t const	total_size = base_size + _cenv_size + _local.size();
	char**			p = new char*[total_size + 1];

	for (size_t i = 0; i < _local.size(); ++i)
		p[i] = _local.at(i).data();
	::memmove(p + _local.size(), base_vars.data(), base_size * sizeof(char*));
	::memmove(p + _local.size() + base_size, _cenv, _cenv_size * sizeof(char*));
	p[total_size] = nullptr;
	return (p);
}
