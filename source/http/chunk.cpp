#include "http/chunk.hpp"
#include "utils/utils.hpp"

#include <string>

using http::Dechunker;

static std::string	_to_string_hex(size_t);
static bool			_check_end(std::istream&);

webserv::Buffer
http::enchunk(webserv::ChunkBuffer const& chbuf) {
	webserv::Buffer	buf;

	buf.push_back(_to_string_hex(chbuf.len()));
	buf.push_back("\r\n");
	buf.push_back(chbuf);
	buf.push_back("\r\n");
	return (buf);
}

static std::string
_to_string_hex(size_t num) {
	std::ostringstream	oss;

	oss << std::hex << num;
	return (oss.str());
}

Dechunker::Dechunker():
	_size(std::nullopt) {}

Dechunker::Chunk
Dechunker::dechunk(webserv::Buffer const& buf) { // DB: make more efficient?
	buf.put(_buffer);
	if (!_size && _get_size() == false)
		return (std::nullopt);
	if (_buffer.str().size() < *_size + 2) // trailing CRLF must be processed
		return (std::nullopt);

	std::string	chunk;

	for (size_t i = 0; i < *_size; ++i)
		chunk.push_back(_buffer.get());
	if (_check_end(_buffer) == false)
		throw (ChunkException("chunk size mismatch"));
	_size = std::nullopt;
	return (chunk);
}

bool
Dechunker::_get_size() {
	std::string	line;

	try {
		utils::getline<"\r\n">(_buffer, line);
		try {
			_size = std::stoul(line);
			return (true);
		} catch (std::out_of_range&) {
			throw (ChunkException("bad chunk size format"));
		}
	} catch (utils::IncompleteLineException&) {
		_size = std::nullopt;
		return (false); // and try again after next read
	}
}

static bool
_check_end(std::istream& is) {
	std::string	str;

	utils::getline<"\r\n">(is, str);
	return (!is.eof() && str.size() == 0);
}

/* ChunkException */

Dechunker::ChunkException::ChunkException(char const* msg):
	_msg(msg) {}

char const*
Dechunker::ChunkException::what() const noexcept {
	return (_msg);
}
