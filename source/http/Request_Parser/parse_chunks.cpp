#include "http/Request.hpp"

#include <string>

using http::Request;
using Parser = Request::Parser;

static size_t	_parse_chunk_size(std::iostream&);
static size_t	_read_chunk(std::istream&, std::string&, size_t);

void
Parser::_parse_chunks(std::iostream& ios, std::string& body) {
	if (_body_length == 0) {
		_body_length = _parse_chunk_size(ios);
		if (_body_length == 0)
			_state = Parser::State::done;
	} else
		_body_length -= _read_chunk(ios, body, _body_length);
}

static size_t
_parse_chunk_size(std::iostream& ios) {
	std::string	line;

	Parser::getline(ios, line);
	return (std::stoul(line));
}

static size_t
_read_chunk(std::istream& is, std::string& body, size_t bytes_max) {
	size_t	bytes_read = 0;

	while (!is.eof() && bytes_read < bytes_max) {
		body.push_back(is.get());
		++bytes_read;
	}
	is.clear();
	return (bytes_read);
}
