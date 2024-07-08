#include "http.hpp"
#include "http/parse.hpp"

#include <iostream>

using http::parse::Parser;

// Basic operations

Parser::Parser():
	_state(State::start) {}

// Modifiers

void
Parser::clear() noexcept {
	_state = State::start;
	_header_buffer.clear();
}

// Accessors

Parser::State
Parser::state() const noexcept {
	return (_state);
}

// Other

Parser::State
Parser::parse(std::iostream& ios, Request& req) {
	try {
		while (!ios.eof()) {
			switch (_state) {
			case State::done:
				return (_state);
			case State::start:
				req = _parse_start(ios);
				break;
			case State::header:
				_parse_headers(ios, req);
				break;
			}
		}
	} catch (IncompleteLineException&) {}
	return (_state);
}

std::iostream&
Parser::getline(std::iostream& ios, std::string& line) {
	http::getline(ios, line);
	if (!ios.eof())
		return (ios);
	ios.clear();
	ios << line;
	line.clear();
	throw (IncompleteLineException());
}
