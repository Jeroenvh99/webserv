#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"
# include "CGI.hpp"
# include "http/Request.hpp"
# include "http/Response.hpp"
# include "route.hpp"

# include "network/StreamSocket.hpp"
# include "network/Address_IPv4.hpp"
# include "network/Handle.hpp"

# include <unordered_map>
# include <utility>

class ClientImpl {
public:
	enum class State;
	using Socket = network::StreamSocket<network::Domain::ipv4>;
	using Address = Socket::Address;

	ClientImpl() = default;
	ClientImpl(Address const&);

private:
	friend class Client;

	State					_state;
	std::stringstream		_buffer;
	http::Request::Parser	_parser;
	http::Request			_request;
	http::Response			_response;
	CGI						_cgi;
	Address					_address;
}; // class ClientImpl

enum class ClientImpl::State {
	idle,
	parse,	// parsing a request
	fetch,	// about to fetch a resource
	wait,	// waiting for CGI processing
	send,	// sending response
}; // enum class ClientImpl::State

using ClientMap = std::unordered_map<network::SharedHandle, ClientImpl>;

class Client {
public:
	using Address = ClientImpl::Address;
	using Socket = ClientImpl::Socket;
	using SocketBox = network::SharedHandle;
	using State = ClientImpl::State;

	Client(ClientMap::value_type&);
	Client(SocketBox const&, ClientImpl&);

	void	operator<<(http::Request const&);
	void	operator<<(http::Response const&);

	Address const&			address() const noexcept;
	Socket const&			socket() const noexcept;
	SocketBox const&		socket_box() const noexcept;
	http::Request const&	request() const noexcept;
	CGI&					cgi() noexcept;
	http::Response const&	response() const noexcept;
	State					state() const noexcept;

	void	parse();
	void	fetch(route::Location const&, Environment const&);
	size_t	recv();
	size_t	send();
	size_t	wait();
	void	clear() noexcept;

private:
	SocketBox	_socket;
	ClientImpl&	_impl;
}; // class Client

#endif // CLIENT_HPP
