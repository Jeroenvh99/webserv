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

http::Response const&
Client::response() const noexcept {
	return (_data._response);
}

Client::State
Client::state() const noexcept {
	return (_data._state);
}
