#include "Client.hpp"

ClientImpl::ClientImpl(Address const& address):
	_state(State::idle),
	_address(address) {}