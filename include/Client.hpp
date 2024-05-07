#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "network/StreamSocket.hpp"
# include "network/Handle.hpp"
# include "http/Request.hpp"

class Client {
public:
	using Socket = network::StreamSocket<network::Domain::ipv4>;
	using SocketBox = network::SharedHandle;

	Client(Socket&&);

	Socket const&			socket() const noexcept;
	SocketBox const&		socket_box() const noexcept;
	http::Request const&	request() const noexcept;
	http::Request&			request() noexcept;

private:
	SocketBox		_socket_box;
	http::Request	_request;
}; // class Client

#endif // CLIENT_HPP
