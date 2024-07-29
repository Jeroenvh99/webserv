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

Client::InputState
Client::istate() const noexcept {
	return (_impl._istate);
}

Client::OutputState
Client::ostate() const noexcept {
	return (_impl._ostate);
}

Worker const&
Client::worker() const noexcept {
	return (_impl._worker);
}

Client::Address const&
Client::address() const noexcept {
	return (_impl._address);
}

// Modifiers

bool
Client::timeout(double interval) const noexcept {
	return (::difftime(webserv::Time(), _impl._last_read) >= interval);
}