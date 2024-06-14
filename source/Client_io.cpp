#include "Client.hpp"

void
Client::operator<<(http::Request const& that) {
	_data._request = that;
	_data._state = State::fetch;
}

void
Client::operator<<(http::Response const& that) {
	_data._buffer.str("");
	_data._buffer.clear();
	_data._buffer << that;
	_data._state = State::send;
}
