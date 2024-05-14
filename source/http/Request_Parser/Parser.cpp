#include "http/Request.hpp"

#include <iostream>

using http::Request;
using Parser = Request::Parser;
using http::Method;
using http::Version;

// Basic operations

Parser::Parser():
	_state(State::start) {}

// Modifiers

void
Parser::append(Buffer const& src) {
	if (src.size() > 0)
		_buffer.clear();
	for (auto const& c: src)
		_buffer << c;
}

void
Parser::clear() noexcept {
	_state = State::start;
	_tmp_hdr.first.clear();
	_tmp_hdr.second.clear();
	_body_length = 0;
	_chunk_length = 0;
	_buffer.str("");
}

// Accessors

Parser::State
Parser::state() const noexcept {
	return (_state);
}

// Other

void
Parser::parse(Buffer const& src, Request& req) {
	append(src);
	try {
		while (!_buffer.eof()) {
			switch (_state) {
			case State::done:
				req.clear();
				clear();
				break;
			case State::start:
				req = parse_start();
				break;
			case State::header:
				parse_headers(req);
				break;
			default:	// body
				parse_body(req);
				break;
			}
		}
	} catch (IncompleteLineException&) {
		return;
	}
}
	
