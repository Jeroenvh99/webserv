#include "Client.hpp"
#include "network/Buffer.hpp"

// Modifiers

void
Client::clear() noexcept {
	_data._buffer.str("");
	_data._buffer.clear();
	_data._state = State::idle;
	_data._parser.clear();
	_data._request.clear();
	_data._response.clear();
}

void
Client::parse() {
	_data._parser.parse(_data._buffer, _data._request);
	if (_data._parser.state() == http::Request::Parser::State::done)
		_data._state = State::fetch;
}

size_t
Client::wait() {
	/* implement */
	_data._state = State::send;
	return (0);
}

size_t
Client::recv() {
	network::Buffer<512>	socket_buffer;
	
	socket().read(socket_buffer);
	_data._buffer << socket_buffer;
	return (socket_buffer.len());
}

size_t
Client::send() {
	network::Buffer<512>	socket_buffer;

	_data._buffer >> socket_buffer;
	if (_data._buffer.eof()) {
		clear();
		_data._state = Client::State::idle;
	}
	return (socket().write(socket_buffer));
}
