#include "http.hpp"
#include "http/parse.hpp"

using http::Method;
using http::Version;
using http::parse::Parser;

static std::string	_get_start_line(std::iostream&);

http::Request
Parser::_parse_start(std::iostream& ios) {
	std::istringstream	iss(_get_start_line(ios));
	std::string			s;

	std::getline(iss, s, ' ');
	Method const	method = to_method(s);
	std::getline(iss, s, ' ');
	URI				uri(s);
	std::getline(iss, s, ' ');
	Version const	version = to_version(s);
	if (version != http::one_one)
		throw (VersionException("unsupported HTTP version"));
	if (!iss.eof())
		throw (Exception("excess elements in first line"));
	_state = State::header;
	return (Request(method, version, std::move(uri)));
}

static std::string
_get_start_line(std::iostream& ios) {
	std::string	line;

	Parser::getline(ios, line);
	while (line.size() == 0)
		Parser::getline(ios, line);	// ignore leading bare CRLF
	return (line);
}