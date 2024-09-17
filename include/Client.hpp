#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

# include "Buffer.hpp"
# include "http/Body.hpp"
# include "http/chunk.hpp"
# include "http/Request.hpp"
# include "http/Response.hpp"
# include "http/parse.hpp"
# include "job/job.hpp"
# include "utils/time.hpp"
# include "route.hpp"
# include "Worker.hpp"

# include "network/StreamSocket.hpp"
# include "network/Address_IPv4.hpp"
# include "network/Handle.hpp"

# include <exception>
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
	http::parse::HeaderParser	_header_parser; // "
	size_t						_body_size;		// union
	http::Dechunker				_dechunker;		// "
	http::Request				_request;
	http::Response				_response;
	http::Body					_response_body;
	Address						_address;
	Worker						_worker;
	webserv::Time				_last_read;
}; // class ClientImpl

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
	enum class OperationStatus;

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

	void	respond(job::Job const&);
	void	respond(job::ErrorJob const&);
  	void	respond(job::RedirectionJob const& job);

	bool			parse_request(webserv::Buffer&);
	bool			parse_response(webserv::Buffer const&);
	OperationStatus	dechunk_and_deliver(webserv::Buffer&);
	OperationStatus	deliver(webserv::Buffer const&);
	OperationStatus	fetch(webserv::Buffer&);
	OperationStatus	fetch(webserv::ChunkBuffer&);

	bool	timeout(double) const noexcept;

	class RedirectionException : std::exception{
		private:
			std::string		_message;
		public:
			RedirectionException(const http::Status status);
			const char *what() const throw();
	};

	class ErrorException : std::exception {
		private:
			std::string		_message;
		public:
			ErrorException(const http::Status status);
			const char *what() const throw();
	};

	class BodySizeException : std::exception {
		private:
			std::string		_message;
		public:
			BodySizeException();
			const char *what() const throw();
	};

private:
	SocketBox	_socket;
	ClientImpl&	_impl;
}; // class Client

enum class Client::OperationStatus {
	pending,
	success,
	failure,
	timeout,
}; // enum class Client::OperationStatus

#endif // CLIENT_HPP
