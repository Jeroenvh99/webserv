#include "http/Request.hpp"

#include <iostream>
#include <sstream>

using http::Request;
using Parser = Request::Parser;

static Parser::State	_parse_body_by_length(std::istream&, std::string&, size_t);
static Parser::State	_parse_body_chunked(std::istream&, std::string&, size_t&);
static Parser::State	_parse_body_until_eof(std::istream&, std::string&);

void
Parser::parse_body(std::iostream& ios, Request& req) {
	switch (_state) {
	case State::body_by_length:
		_state = _parse_body_by_length(ios, req.body(), _body_length);
		break;
	case State::body_chunked:
		_state = _parse_body_chunked(ios, req.body(), _chunk_length);
		break;
	case State::body_until_eof:
		_state = _parse_body_until_eof(ios, req.body());
		break;
	default:
		break;
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
