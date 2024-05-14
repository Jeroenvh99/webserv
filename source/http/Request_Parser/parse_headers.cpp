#include "http/Request.hpp"

#include <sstream>

using http::Request;
using Parser = Request::Parser;

static bool	_get_header_line(std::string&, std::stringstream&);

void
Parser::parse_headers(Request& req) {
	std::string	s;

	while (_get_header_line(s, _buffer))
		_parse_header(req, s);
	if (_tmp_hdr.first.size() != 0) {	// make temporary header definitive
		req.header_add(std::move(_tmp_hdr));
		_tmp_hdr.first.clear();
		_tmp_hdr.second.clear();
	}
	needs_body(req);
}

static bool
_get_header_line(std::string& line, std::stringstream& buffer) {
	http::getline(buffer, line);
	if (line.size() == 0) {	// bare CRLF returns false
		if (buffer.eof())	// incomplete line throws
			throw (Parser::IncompleteLineException());
		return (false);
	}
	if (buffer.eof()) {	// incomplete line throws
		buffer << line;
		buffer.clear();
		throw (Parser::IncompleteLineException());
	}
	return (true);
}

static bool			_is_continuation(std::string const&);
static std::string	_parse_key(std::istream&);

void
Parser::_parse_header(Request& req, std::string const& str) {
	if (_is_continuation(str)) {	// append continuation lines
		_tmp_hdr.second += ' ';
		_tmp_hdr.second += trim_ws(std::string(str)); // can be optimized
	} else if (_tmp_hdr.first.size() != 0) { // make temporary header definitive
		req.header_add(std::move(_tmp_hdr));
		_tmp_hdr.first.clear();
		_tmp_hdr.second.clear();
	} else {						// reinitialize temporary header
		std::istringstream	iss(str);

		_tmp_hdr.first = _parse_key(iss);
		iss >> _tmp_hdr.second;
		trim_ws(_tmp_hdr.second);
	}	
}

static bool
_is_continuation(std::string const& str) {
	char const	c = str[0];

	return (c == ' ' || c == '\t');
}

static std::string
_parse_key(std::istream& is) {
	std::string	s;

	std::getline(is, s, ':');
	if (is.eof())
		throw (Parser::HeaderException("bad header format"));
	return (s);
}
