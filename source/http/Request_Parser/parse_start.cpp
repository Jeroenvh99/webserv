#include "http.hpp"
#include "http/Request.hpp"

#include <iostream>

using http::Request;
using Parser = Request::Parser;
using http::Method;
using http::Version;

static std::string	_get_start_line(std::iostream&);
static Method		_parse_method(std::string const&);
static Version		_parse_version(std::string const&);

Request
Parser::parse_start() {
	std::istringstream	iss(_get_start_line(_buffer));
	std::string			s;

	std::getline(iss, s, ' ');
	Method const	method = _parse_method(s);
	std::getline(iss, s, ' ');
	Version const	version = _parse_version(s);
	std::getline(iss, s, ' ');
	if (!iss.eof())
		throw (StartLineException("excess elements in first line"));
	_state = State::header;
	return (Request(method, version, std::move(s)));
}

static std::string
_get_start_line(std::iostream& buffer) {
	std::string	res;

	http::getline(buffer, res);
	while (res.size() == 0)
		http::getline(buffer, res);	// ignore leading bare CRLF
	if (buffer.eof()) {			// line must end with CRLF
		buffer << res;
		buffer.clear();
		throw (Parser::IncompleteLineException());
	}
	return (res);
}

static Method
_parse_method(std::string const& str) {
	try {
		return (http::method_from_string(str));
	} catch (std::invalid_argument& e) {
		throw (Parser::MethodException(e.what()));
	}
}

static Version
_parse_version(std::string const& str) {
	try {
		Version const	version = http::version_from_string(str);

		if (version != http::one_zero || version != http::one_one)
			throw (Parser::VersionException("unsupported HTTP version"));
		return (version);
	} catch (std::exception& e) {
		throw (Parser::VersionException(e.what()));
	}
}
