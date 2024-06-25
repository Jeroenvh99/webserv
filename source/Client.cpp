#include "Client.hpp"

// Basic operations

Client::Client(ClientMap::value_type& pair):
	_socket(pair.first), _impl(pair.second) {}

Client::Client(SocketBox const& socket, ClientImpl& data):
	_socket(socket), _impl(data) {}

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
	return (_impl._request);
}

http::Response const&
Client::response() const noexcept {
	return (_impl._response);
}

Client::State
Client::state() const noexcept {
	return (_impl._state);
}

Worker const&
Client::worker() const noexcept {
	return (_impl._worker);
}

Client::Address const&
Client::address() const noexcept {
	return (_impl._address);
}