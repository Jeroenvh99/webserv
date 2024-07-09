#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"
# include "Buffer.hpp"
# include "Worker.hpp"
# include "job/job.hpp"
# include "http/Body.hpp"
# include "http/Request.hpp"
# include "http/Response.hpp"
# include "http/parse.hpp"
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

	void	_clear() noexcept;

	void	_buffer_fill(std::string const&);
	size_t	_buffer_flush(std::string&);
	size_t	_buffer_flush(webserv::Buffer&);
	void	_buffer_empty() noexcept;

	State						_state;
	std::stringstream			_buffer;
	http::parse::RequestParser	_parser;		// union
	http::parse::HeaderParser	_header_parser; // union
	http::Request				_request;
	http::Body					_request_body;
	http::Response				_response;
	http::Body					_response_body;
	Address						_address;
	Worker						_worker;
}; // class ClientImpl

enum class ClientImpl::State {
	idle,
	parse_request,  // receiving and parsing request line and headers
	parse_response, // receiving request body and/or parsing response headers
	work,          	// receiving request body and/or sending response
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

	Address const&			address() const noexcept;
	Socket const&			socket() const noexcept;
	SocketBox const&		socket_box() const noexcept;
	http::Request const&	request() const noexcept;
	http::Response const&	response() const noexcept;
	State					state() const noexcept;
	Worker const&			worker() const noexcept;

	bool		parse_request(webserv::Buffer&);
	bool		parse_response(webserv::Buffer const&);
	job::Status	respond(job::Job const&);
	job::Status	respond(job::ErrorJob const&);
	job::Status	deliver(webserv::Buffer const&);
	job::Status	fetch(webserv::Buffer&);
	job::Status	wait();

private:
	SocketBox	_socket;
	ClientImpl&	_impl;
}; // class Client

#endif // CLIENT_HPP
