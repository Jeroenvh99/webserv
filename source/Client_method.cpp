#include "Client.hpp"
#include "network/Buffer.hpp"

// Modifiers

void
Client::parse() {
	_data._parser.parse(_data._buffer, _data._request);
	if (_data._parser.state() == http::Request::Parser::State::done)
		_data._state = State::fetch;
}

void
Client::fetch() {
	// if CGI should be executed
	// setup connection between server and CGI executable
	// exec CGI
	// _state = State::wait;
	*this << http::Response("David is an absolute genius", http::Status::ok);
	_data._state = State::send;
}

size_t
Client::wait() {
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
	if (_data._buffer.eof())
		_data._state = Client::State::idle;
	return (socket().write(socket_buffer));
}
