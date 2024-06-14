#include "Client.hpp"
#include "network/Buffer.hpp"

// Modifiers

void
Client::clear() noexcept {
	_impl._state = State::idle;
	_impl._buffer.str("");
	_impl._buffer.clear();
	_impl._parser.clear();
	_impl._request.clear();
	_impl._response.clear();
}

void
Client::parse() {
	_impl._parser.parse(_impl._buffer, _impl._request);
	if (_impl._parser.state() == http::Request::Parser::State::done)
		_impl._state = State::fetch;
}

size_t
Client::wait() {
	/* implement */
	_impl._state = State::send;
	return (0);
}

size_t
Client::recv() {
	network::Buffer<512>	socket_buffer;
	
	socket().read(socket_buffer);
	_impl._buffer << socket_buffer;
	return (socket_buffer.len());
}

size_t
Client::send() {
	network::Buffer<512>	socket_buffer;

	_impl._buffer >> socket_buffer;
	if (_impl._buffer.eof()) {
		clear();
		_impl._state = Client::State::idle;
	}
	return (socket().write(socket_buffer));
}
