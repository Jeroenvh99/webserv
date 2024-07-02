#include "http/Message.hpp"

#include <sstream>

using http::Header;

std::string
http::to_string(Header const& hdr) {
	return (hdr.first + ": " + hdr.second);
}

Header
http::to_header(std::string const& str) {
	std::istringstream	iss(str);
	std::string			name;
	std::string			value;
	
	std::getline(iss, name, ':');
	std::getline(iss, value);
	return {name, value};
}