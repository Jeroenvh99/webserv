#include "Client.hpp"

ClientImpl::ClientImpl(Address const& address):
	_state(State::idle),
	_cgi{std::string(address)},
	_address(address) {}