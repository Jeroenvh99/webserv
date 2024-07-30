#include "http/chunk.hpp"
#include "http/parse.hpp"

#include <string>

using http::Dechunker;

static std::string	_to_string_hex(size_t);

webserv::Buffer&
http::enchunk(webserv::Buffer& wsbuf, webserv::ChunkBuffer const& chbuf) {
	wsbuf.push_back(_to_string_hex(chbuf.len()));
	wsbuf.push_back("\r\n");
	wsbuf.push_back(chbuf);
	wsbuf.push_back("\r\n");
	return (wsbuf);
}

static std::string
_to_string_hex(size_t num) {
	std::ostringstream	oss;

	oss << std::hex << num;
	return (oss.str());
}

Dechunker::Dechunker():
	_chunk_size(std::nullopt), _bytes_dechunked(0) {}

std::ostream&
Dechunker::buffer() noexcept {
	return (_buf);
}

Dechunker::Result
Dechunker::dechunk(webserv::Buffer& wsbuf) {
	if (_chunk_size && _bytes_dechunked + wsbuf.len() <= _chunk_size)
		return (_bytes_dechunked += wsbuf.len(), std::nullopt);
	_buf.clear();
	_buf << wsbuf;
	wsbuf.empty();
	
	Result	res = dechunk(wsbuf, _buf);

	if (res && *res == 0 && !_buf.eof())
		throw (Exception("buffer contains bytes past final chunk"));
	return (res);
}

Dechunker::Result
Dechunker::dechunk(webserv::Buffer& wsbuf, std::istream& is) {
	Result	res = std::nullopt;
	
	while (is) {
		res = dechunk_single(wsbuf, is);
		if (!res)
			break;
	}
	return (res);
}

Dechunker::Result
Dechunker::dechunk_single(webserv::Buffer& wsbuf, std::istream& is) {
	try {
		if (!_chunk_size)
			get_size(is);

		while (_bytes_dechunked < *_chunk_size) {
			if  (wsbuf.len() == wsbuf.capacity())
				return (std::nullopt);

			char const	c = is.get();

			if (is.eof())
				return (std::nullopt);
			if (!wsbuf.push_back(c))
				throw (std::out_of_range("Dechunker::dechunk_single"));
			++_bytes_dechunked;
		}
		return (get_end(is));
	} catch (utils::IncompleteLineException&) {
		return (std::nullopt);
	}
}

void
Dechunker::get_size(std::istream& is) {
	std::string	s;

	try {
		utils::getline<"\r\n">(is, s);
		_chunk_size = std::stoul(s, nullptr, 16);
		_bytes_dechunked = 0;
	} catch (std::out_of_range&) {
		throw (Exception("invalid chunk size format"));
	}
}

size_t
Dechunker::get_end(std::istream& is) {
	std::string	s;

	utils::getline<"\r\n">(is, s);
	if (s.size() > 0)
		throw (Exception("incorrectly sized chunk"));
	_chunk_size = std::nullopt;
	return (_bytes_dechunked);
}

/* Exception */

Dechunker::Exception::Exception(char const* msg):
	_msg(msg) {}

char const*
Dechunker::Exception::what() const noexcept {
	return (_msg);
}