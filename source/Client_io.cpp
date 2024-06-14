#include "Client.hpp"

void
Client::operator<<(http::Request const& that) {
	_impl._request = that;
	_impl._state = State::fetch;
}

void
Client::operator<<(http::Response const& that) {
	_impl._buffer.str("");
	_impl._buffer.clear();
	_impl._buffer << that;
	_impl._state = State::send;
}
