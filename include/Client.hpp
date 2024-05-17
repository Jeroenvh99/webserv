#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "network/StreamSocket.hpp"
# include "network/Handle.hpp"
# include "http/Request.hpp"
# include "http/Response.hpp"

# include <unordered_map>
# include <utility>

class ClientData {
public:
	enum class State;

private:
	friend class Client;

	State					_state;
	std::stringstream		_buffer;
	http::Request			_request;
	http::Request::Parser	_parser;
}; // class ClientData

enum class ClientData::State {
	idle,
	parse,	// parsing a request
	wait,	// waiting for request processing
	send,	// sending response
}; // enum class ClientData::State

using ClientMap = std::unordered_map<network::SharedHandle, ClientData>;

class Client {
public:
	using State = ClientData::State;
	using Socket = network::StreamSocket<network::Domain::ipv4>;
	using SocketBox = network::SharedHandle;

	Client(ClientMap::value_type&);
	Client(SocketBox const&, ClientData&);

	void	operator<<(http::Response const&);

	Socket const&			socket() const noexcept;
	SocketBox const&		socket_box() const noexcept;
	http::Request const&	request() const noexcept;
	http::Request&			request() noexcept; // remove?
	State					state() const noexcept;

	void	parse();
	size_t	recv();
	size_t	send();

private:
	SocketBox	_socket;
	ClientData&	_data;
}; // class Client

#endif // CLIENT_HPP
