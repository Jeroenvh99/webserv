#include "Client.hpp"

// Modifiers

void
Client::clear() noexcept {
	_impl._state = State::idle;
	_impl._reset_buffer();
	_impl._parser.clear();
	_impl._request.clear();
	_impl._response.clear();
}