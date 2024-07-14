#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"
# include "Buffer.hpp"
# include "Worker.hpp"
# include "job/job.hpp"
# include "http/Body.hpp"
# include "http/chunk.hpp"
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
	enum class InputState;
	enum class OutputState;

	using Socket = network::StreamSocket<network::Domain::ipv4>;
	using Address = Socket::Address;

	ClientImpl() = default;
	ClientImpl(Address const&);

private:
	friend class Client;

	void	_clear() noexcept;

	void	_buffer_fill(std::string const&);
	void	_buffer_fill(webserv::Buffer const&);
	size_t	_buffer_flush(std::string&);
	size_t	_buffer_flush(webserv::Buffer&);
	void	_buffer_empty() noexcept;

	InputState					_istate;
	OutputState					_ostate;
	std::stringstream			_buffer;
	http::parse::RequestParser	_parser;		// union
	http::parse::HeaderParser	_header_parser; // union
	http::Dechunker				_dechunker;
	http::Request				_request;
	http::Response				_response;
	http::Body					_response_body; // delete this: ostate will determine what should be done with worker output
	Address						_address;
	Worker						_worker;
}; // class ClientImpl

// enum class ClientImpl::State {
// 	idle,
// 	parse_request,  // receiving and parsing request line and headers
// 	parse_response, // receiving request body and/or parsing response headers
// 	work,          	// receiving request body and/or sending response
// }; // enum class ClientImpl::State

enum class ClientImpl::InputState {
	parse_request,	// directing input to request parser
	dechunk,		// directing input to dechunker
	deliver,		// directing input to request worker
	closed,			// not accepting further input
}; // enum class ClientImpl::InputState

enum class ClientImpl::OutputState {
	parse_response,	// directing worker output to response parser
	fetch,			// directing worker output to socket
	enchunk,		// enchunk worker output and direct it to socket
	closed,			// not expecting further output (might need this to keep faulty CGI in check)
}; // enum class ClientImpl::OutputState

using ClientMap = std::unordered_map<network::SharedHandle, ClientImpl>;

class Client {
public:
	using Address = ClientImpl::Address;
	using Socket = ClientImpl::Socket;
	using SocketBox = network::SharedHandle;
	using InputState = ClientImpl::InputState;
	using OutputState = ClientImpl::OutputState;

	Client(ClientMap::value_type&);
	Client(SocketBox const&, ClientImpl&);

	Address const&			address() const noexcept;
	Socket const&			socket() const noexcept;
	SocketBox const&		socket_box() const noexcept;
	http::Request const&	request() const noexcept;
	http::Response const&	response() const noexcept;
	InputState				istate() const noexcept;
	OutputState				ostate() const noexcept;
	Worker const&			worker() const noexcept;

	bool		parse_request(webserv::Buffer&);
	bool		parse_response(webserv::Buffer const&);
	job::Status	respond(job::Job const&);
	job::Status	respond(job::ErrorJob const&);
	job::Status	deliver(webserv::Buffer const&);
	job::Status	dechunk(webserv::Buffer const&);
	job::Status	fetch(webserv::Buffer&);
	job::Status	fetch(http::ChunkBuffer&);
	job::Status	wait();

private:
	SocketBox	_socket;
	ClientImpl&	_impl;
}; // class Client

#endif // CLIENT_HPP
