#include "http/chunk.hpp"
#include "utils/utils.hpp"

#include <string>

using http::Dechunker;

static std::string	_to_string_hex(size_t);

static std::string
_to_string_hex(size_t num) {
	std::ostringstream	oss;

	oss << std::hex << num;
	return (oss.str());
}

/* Dechunker */

// Basic operations

Dechunker::Dechunker():
	_buf(""), _remaining(std::nullopt), _found_null(false) {}

// Utility methods

void
Dechunker::clear() noexcept {
	_buf.str() = "";
	_buf.clear();
	_remaining = std::nullopt;
	_found_null = false;
}

Dechunker::Status
Dechunker::dechunk(webserv::Buffer& wsbuf) {
	if (_remaining && wsbuf.len() <= _remaining) // wsbuf fits entirely within the current chunk, so no processing is required
		return (*_remaining -= wsbuf.len(), Status::pending);
	_buf.clear();
	_buf << wsbuf;
	wsbuf.empty(); // move the contents of wsbuf into internal buffer
	
	Status	status = dechunk_core(wsbuf);

	if (status == Status::done) {
		if (!_buf.eof())
			throw (Exception("buffer contains bytes past final chunk"));
		clear();
	}
	return (status);
}

Dechunker::Status
Dechunker::dechunk_core(webserv::Buffer& wsbuf) {
	Status	res = Status::pending;
	
	while (_buf) {
		res = dechunk_one(wsbuf);
		if (res == Status::done)
			break;
	}
	return (res);
}

Dechunker::Status
Dechunker::dechunk_one(webserv::Buffer& wsbuf) {
	try {
		if (!_remaining) // a new chunk is about to be processed
			extract_size(); 
		while (*_remaining) {
			if (wsbuf.len() == wsbuf.capacity()) // no more bytes can be put back into wsbuf
				return (Status::pending);

			char const	c = _buf.get();

			if (_buf.eof()) // internal buffer has been fully processed
				return (Status::pending);
			wsbuf.push_back(c);
		}
		extract_terminator();
		// get_end(is);
		return (_found_null ? Status::done : Status::pending);
	} catch (utils::IncompleteLineException&) { // size indicator or CRLF terminator could not be fully extracted
		return (Status::pending);
	}
}

void
Dechunker::extract_size() {
	std::string	s;

	try {
		utils::getline<"\r\n">(_buf, s);
		_remaining = std::stoul(s, nullptr, 16);
		if (*_remaining == 0)
			_found_null = true;
	} catch (std::out_of_range&) {
		throw (Exception("invalid chunk size format"));
	}
}

void
Dechunker::extract_terminator() {
	std::string	s;

	utils::getline<"\r\n">(_buf, s);
	if (s.length() > 0)
		throw (Exception("incorrectly sized chunk"));
	// _chunk_size = std::nullopt;
}

std::string
Dechunker::getbuf() const {
	return _buf.str();
}

/* Dechunker::Exception */

Dechunker::Exception::Exception(char const* msg):
	_msg(msg) {}

char const*
Dechunker::Exception::what() const noexcept {
	return (_msg);
}

/* enchunk */

// webserv::Buffer&
// http::enchunk(webserv::Buffer& wsbuf, http::ChunkBuffer const& chbuf) {
// 	wsbuf.push_back(_to_string_hex(chbuf.len()));
// 	wsbuf.push_back("\r\n");
// 	wsbuf.push_back(chbuf);
// 	wsbuf.push_back("\r\n");
// 	return (wsbuf);
// }
