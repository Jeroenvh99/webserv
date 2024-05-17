#include "Client.hpp"
#include "network/Buffer.hpp"

using Buffer = network::Buffer<512>;

void
Client::operator<<(http::Response const& that) {
	_data._buffer.str("");
	_data._buffer.clear();
	_data._buffer << that;
	_data._state = Client::State::send;
}

size_t
Client::recv() {
	Buffer			socket_buffer;
	
	socket().read(socket_buffer);
	_data._buffer << socket_buffer;
	return (socket_buffer.len());
}

size_t
Client::send() {
	Buffer	socket_buffer;

	_data._buffer >> socket_buffer;
	if (_data._buffer.eof())
		_data._state = Client::State::idle;
	return (socket().write(socket_buffer));
}
