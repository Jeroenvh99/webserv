#include "Client.hpp"

Client::Client(ClientMap::value_type& pair):
	_socket(pair.first), _data(pair.second) {}

Client::Client(SocketBox const& socket, ClientData& data):
	_socket(socket), _data(data) {}

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
	return (_data.request());
}

http::Request&
Client::request() noexcept {
	return (_data.request());
}

http::Request::Parser const&
Client::parser() const noexcept {
	return (_data.parser());
}

http::Request::Parser&
Client::parser() noexcept {
	return (_data.parser());
}

Client::State
Client::state() const noexcept {
	return (_data.state());
}
