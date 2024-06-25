#include "http/Dechunker.hpp"

#include <string>

using http::Dechunker;

static bool	_check_end(std::istream&);

Dechunker::Dechunker:
	_remaining(std::nullopt_t) {}

Dechunker::Chunk
Dechunker::dechunk(webserv::Buffer const& buf) { // DB: make more efficient?
	buf.write(_buffer);
	if (!size && _get_size == false)
		return (std::nullopt_t);
	if (_buffer.str().size < *_size + 2) // trailing CRLF must be processed
		return (std::nullopt_t);

	std::string	chunk;

	for (size_t i = 0; i < *_size; ++i)
		chunk.push_back(_buffer.get());
	if (_check_end(_buffer) == false)
		throw (ChunkError("bad chunk size"));
	_size = std::nullopt_t;
	return (chunk);
}

bool
Dechunker::_get_size() {
	std::string	line;

	http::getline(_buffer, line);
	if (!_buffer.eof()) { // a CRLF-delimited line was read
		try {
			_size = std::stoul(line);
			return (true);
		} catch (std::out_of_range&) {
			throw (ChunkException("bad chunk size format"));
		}
	} // put everything back and try again after next read
	_buffer.clear();
	_buffer << line;
	_remaining = std::nullopt_t;
	return (false);
}

static bool
_check_end(std::istream& is) {
	std::string	str;

	http::getline(is, str);
	return (!is.eof() && str.size() == 0);
}

/* ChunkException */

Dechunker::ChunkException::ChunkException(char const* msg):
	_msg(msg) {}

Dechunker::ChunkException::what() const noexcept {
	return (_msg);
}