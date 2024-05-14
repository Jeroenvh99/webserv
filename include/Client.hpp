#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "network/StreamSocket.hpp"
# include "network/Handle.hpp"
# include "http/Request.hpp"

# include <unordered_map>
# include <utility>

class ClientData {
public:
	enum class State;

	http::Request const&			request() const noexcept;
	http::Request&					request() noexcept;
	http::Request::Parser const&	parser() const noexcept;
	http::Request::Parser&			parser() noexcept;
	State							state() const noexcept;

private:
	State					_state;
	http::Request			_request;
	http::Request::Parser	_parser;
}; // class ClientData

enum class ClientData::State {
	idle,
	parse,
	wait,
	send,
}; // enum class ClientData::State

using ClientMap = std::unordered_map<network::SharedHandle, ClientData>;

class Client {
public:
	using State = ClientData::State;
	using Socket = network::StreamSocket<network::Domain::ipv4>;
	using SocketBox = network::SharedHandle;

	Client(ClientMap::value_type&);
	Client(SocketBox const&, ClientData&);

	Socket const&					socket() const noexcept;
	SocketBox const&				socket_box() const noexcept;
	http::Request const&			request() const noexcept;
	http::Request&					request() noexcept;
	http::Request::Parser const&	parser() const noexcept;
	http::Request::Parser&			parser() noexcept;
	State							state() const noexcept;
private:
	SocketBox	_socket;
	ClientData&	_data;
}; // class Client

#endif // CLIENT_HPP
