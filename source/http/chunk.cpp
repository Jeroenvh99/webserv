#include "http/chunk.hpp"
#include "utils/utils.hpp"

#include <string>

using http::Dechunker;

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
		if (_buf.peek() != std::stringstream::traits_type::eof())
			throw (Exception("buffer contains bytes past final chunk"));
		clear();
	}
	return (status);
}

Dechunker::Status
Dechunker::dechunk_core(webserv::Buffer& wsbuf) {
	Status	res = Status::pending;
	
	try {
		while (_buf) {
			res = dechunk_one(wsbuf);
			if (res == Status::done)
				break;
		}
	} catch (utils::IncompleteLineException&) { // size indicator or CRLF terminator could not be fully extracted
		return (Status::pending);
	}
	return (res);
}

Dechunker::Status
Dechunker::dechunk_one(webserv::Buffer& wsbuf) {
	if (!_remaining) // a new chunk is about to be processed
		extract_size(); 
	while (*_remaining) {
		if (wsbuf.len() == wsbuf.capacity()) // no more bytes can be put back into wsbuf
			return (Status::pending);

		char const	c = _buf.get();

		wsbuf.push_back(c);
		if (_buf.peek() == std::stringstream::traits_type::eof()) // internal buffer has been fully processed
			return (Status::pending);
		--*_remaining;
	}
	extract_terminator();
	_remaining = std::nullopt;
	return (_found_null ? Status::done : Status::pending);
}

void
Dechunker::extract_size() {
	std::string	s;

	try {
		utils::getline<"\r\n">(_buf, s);
		if (s.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
			throw (Exception("this is not only a hexadecimal number"));
		_remaining = std::stoul(s, nullptr, 16);
		if (*_remaining == 0)
			_found_null = true;
	} catch (std::out_of_range&) {
		throw (Exception("invalid chunk size format"));
	} catch (std::invalid_argument&) {
		throw (Exception("this is not a hexadecimal number"));
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
