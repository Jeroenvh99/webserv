#include "Client.hpp"

Client::Client(Socket&& handle):
	_socket_box(std::make_shared<Socket>(std::move(handle))),
	_request() {}

Client::Socket const&
Client::socket() const noexcept {
	return (static_cast<Socket const&>(*_socket_box));
}

Client::SocketBox const&
Client::socket_box() const noexcept {
	return (_socket_box);
}

http::Request const&
Client::request() const noexcept {
	return (_request);
}

http::Request&
Client::request() noexcept {
	return (_request);
}

http::Request::Parser const&
Client::parser() const noexcept {
	return (_parser);
}

http::Request::Parser&
Client::parser() noexcept {
	return (_parser);
}
