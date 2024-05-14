#include "http.hpp"

#include <sstream>

using http::Version;

Version
http::version_from_string(std::string const& that) {
	std::istringstream	iss(that);
	std::string			s;
	Version				res;

	std::getline(iss, s, '/');
	if (s != "HTTP")
		throw (std::invalid_argument("not a HTTP version"));
	std::getline(iss, s, '.');
	res.first = std::stoul(s);
	std::getline(iss, s, '.');
	res.second = std::stoul(s);
	return (res); // limits check?
}

std::string
http::version_to_string(Version const& that) {
	std::ostringstream	oss;

	oss << "HTTP/"
		<< std::to_string(that.first)
		<< "."
		<< std::to_string(that.second);
	return (oss.str());
}
