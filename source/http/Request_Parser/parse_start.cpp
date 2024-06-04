#include "http.hpp"
#include "http/Request.hpp"

using http::Request;
using Parser = Request::Parser;
using http::Method;
using http::Version;

static std::string	_get_start_line(std::iostream&);
static Method		_parse_method(std::string const&);
static Version		_parse_version(std::string const&);

Request
Parser::_parse_start(std::iostream& ios) {
	std::istringstream	iss(_get_start_line(ios));
	std::string			s;

	std::getline(iss, s, ' ');
	Method const	method = _parse_method(s);
	std::getline(iss, s, ' ');
	URI				uri(s);
	std::getline(iss, s, ' ');
	if (!iss.eof())
		throw (StartLineException("excess elements in first line"));
	Version const	version = _parse_version(s);
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

static Method
_parse_method(std::string const& str) {
	try {
		return (http::to_method(str));
	} catch (std::invalid_argument& e) {
		throw (Parser::MethodException(e.what()));
	}
}

static Version
_parse_version(std::string const& str) {
	try {
		Version const	version = http::to_version(str);

		if (version != http::one_zero && version != http::one_one)
			throw (Parser::VersionException("unsupported HTTP version"));
		return (version);
	} catch (std::exception& e) {
		throw (Parser::VersionException(e.what()));
	}
}
