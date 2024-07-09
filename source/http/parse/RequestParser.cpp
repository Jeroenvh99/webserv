#include "http.hpp"
#include "http/parse.hpp"

#include <iostream>

using http::parse::RequestParser;

static std::string	_get_start_line(std::iostream&);

// Basic operations

RequestParser::RequestParser():
	_state(State::start) {}

// Modifiers

void
RequestParser::clear() noexcept {
	_header_parser.clear();
	_state = State::start;
}

// Accessors

RequestParser::State
RequestParser::state() const noexcept {
	return (_state);
}

// Other

RequestParser::State
RequestParser::parse(std::iostream& ios, Request& req) {
	try {
		while (!ios.eof()) {
			switch (_state) {
			case State::done:
				return (_state);
			case State::start:
				req = _parse_request_line(ios);
				break;
			case State::header:
				if (_header_parser.parse(ios, req.headers()) == HeaderParser::Status::done)
					_state = State::done;
				break;
			}
		}
	} catch (utils::IncompleteLineException&) {}
	return (_state);
}

http::Request
RequestParser::_parse_request_line(std::iostream& ios) {
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

	utils::getline<"\r\n">(ios, line);
	while (line.size() == 0)
		utils::getline<"\r\n">(ios, line);	// ignore leading bare CRLF
	return (line);
}