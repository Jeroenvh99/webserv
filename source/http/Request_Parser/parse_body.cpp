#include "http/Request.hpp"

#include <iostream>
#include <sstream>

using http::Request;
using Parser = Request::Parser;

static bool				_get_body_length(size_t&, Request const&);
static Parser::State	_parse_body_by_length(std::istream&, std::string&, size_t);
static Parser::State	_parse_body_chunked(std::istream&, std::string&, size_t&);
static Parser::State	_parse_body_until_eof(std::istream&, std::string&);

void
Parser::parse_body(Request& req) {
	switch (_state) {
	case State::body_by_length:
		_state = _parse_body_by_length(_buffer, req.body(), _body_length);
		break;
	case State::body_chunked:
		_state = _parse_body_chunked(_buffer, req.body(), _chunk_length);
		break;
	case State::body_until_eof:
		_state = _parse_body_until_eof(_buffer, req.body());
		break;
	default:
		break;
	}
}

bool
Parser::needs_body(Request const& req) {
	if (_get_body_length(_body_length, req)) {
		if (req.has_header("Transfer-Encoding"))
			throw (std::invalid_argument("duplicate body length specification"));
		return (_state = State::body_by_length, true);
	}
	try {
		std::string const&	strval = req.header("Transfer-Encoding");

		if (http::strcmp_nocase(strval, "chunked"))
			return (_state = State::body_chunked, true);
		return (_state = State::body_until_eof, true);
	} catch (std::out_of_range& e) {	// Transfer-Encoding is not defined
		return (_state = Parser::State::done, false);
	}
}

bool
_get_body_length(size_t& len, Request const& req) {
	try {
		std::string const&	strval = req.header("Content-Length");

		try {
			len = std::stoul(strval);
			return (true);
		} catch (std::out_of_range& e) {		// overflow
			throw (std::invalid_argument("bad header Content-Length"));
		} catch (std::invalid_argument& e) {	// non-numeric value
			throw (std::invalid_argument("bad header Content-Length"));
		}
	} catch (std::out_of_range& e) {	// Content-Length is not defined
		return (-1);
	}
}

static Parser::State
_parse_body_by_length(std::istream& is, std::string& body, size_t length) {
	while (!is.eof()) {
		if (body.size() >= length)
			return (Parser::State::done);
		body.push_back(is.get());
	}
	return (Parser::State::body_by_length);
}

static Parser::State
_parse_body_until_eof(std::istream& is, std::string& body) {
	if (is.eof())
		return (Parser::State::done);
	while (!is.eof())
		body.push_back(is.get());
	return (Parser::State::body_until_eof);
}

static Parser::State
_parse_body_chunked(std::istream&, std::string&, size_t&) {
	return (Parser::State::done); // temp
}
