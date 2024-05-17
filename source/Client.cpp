#include "Client.hpp"

// Basic operations

Client::Client(ClientMap::value_type& pair):
	_socket(pair.first), _data(pair.second) {}

Client::Client(SocketBox const& socket, ClientData& data):
	_socket(socket), _data(data) {}

// Accessors

Client::Socket const&
Client::socket() const noexcept {
	return (static_cast<Socket const&>(*_socket));
}

Client::SocketBox const&
Client::socket_box() const noexcept {
	return (_socket);
}

http::Request const&
Client::request() const noexcept {
	return (_data._request);
}

http::Request&
Client::request() noexcept {
	return (_data._request);
}

Client::State
Client::state() const noexcept {
	return (_data._state);
}

// Modifiers

void
Client::parse() {
	_data._parser.parse(_data._buffer, _data._request);
	if (_data._parser.state() == http::Request::Parser::State::done)
		_data._state = State::wait;
}
