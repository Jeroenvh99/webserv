#include "Client.hpp"

ClientImpl::ClientImpl(Address const& address):
	_state(State::idle),
	_address(address) {}

void
ClientImpl::_reset_buffer(std::string const& str) {
	_buffer.clear();
	_buffer.str(str);
}