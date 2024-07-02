#include "Client.hpp"

ClientImpl::ClientImpl(Address const& address):
	_state(State::idle),
	_address(address) {}

void
ClientImpl::_buffer_fill(std::string const& str) {
	_buffer.clear();
	_buffer.str(str);
}

size_t
ClientImpl::_buffer_flush(std::string& str) {
	std::streampos const	pos = _buffer.tellg();

	if (pos != -1) {
		str = _buffer.str().substr(pos);
		_buffer_empty();
		return (str.size());
	}
	return (0);
}

size_t
ClientImpl::_buffer_flush(webserv::Buffer& wsbuf) {
	size_t const	bytes = wsbuf.get(_buffer);

	_buffer_empty();
	return (bytes);
}

void
ClientImpl::_buffer_empty() noexcept {
	_buffer.str("");
	_buffer.clear(std::ios::eofbit);
}
