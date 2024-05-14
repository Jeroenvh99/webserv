#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "network/StreamSocket.hpp"
# include "network/Handle.hpp"
# include "http/Request.hpp"

class Client {
public:
	enum class State;
	using Socket = network::StreamSocket<network::Domain::ipv4>;
	using SocketBox = network::SharedHandle;

	Client(Socket&&);

	Socket const&					socket() const noexcept;
	SocketBox const&				socket_box() const noexcept;
	http::Request const&			request() const noexcept;
	http::Request&					request() noexcept;
	http::Request::Parser const&	parser() const noexcept;
	http::Request::Parser&			parser() noexcept;

private:
	SocketBox				_socket_box;
	http::Request			_request;
	http::Request::Parser	_parser;
}; // class Client

enum class Client::State {
}; // enum class Client::State

#endif // CLIENT_HPP
