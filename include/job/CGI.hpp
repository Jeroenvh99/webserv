#ifndef JOB_CGI_HPP
# define JOB_CGI_HPP

#include "job.hpp"
#include "Buffer.hpp"
#include "network/StreamSocket.hpp"

#include <iostream>

namespace job {
	class CGI: public BaseResource {
	public:
		class WaitException;
		class SocketException;
		class ForkException;
		enum class ProcessStatus;

		CGI(Job const&);
		~CGI();

		size_t			write(webserv::Buffer const&) const;
		size_t			read(webserv::Buffer&) const;
		ProcessStatus	wait();
		ProcessStatus	kill() noexcept;

	private:
		using Socket = network::StreamSocket<network::Domain::local>;

		void			_fork(Job const&);
		void			_redirect_io(Socket&);
		void			_exec(Job const&);
		Socket const&	socket() const;

		static constexpr pid_t	_no_child = 0;

		pid_t					_pid;
		network::SharedHandle	_socket;
		std::string				_obuf;
	}; // class CGI

	class CGI::WaitException: public CGI::Exception {
	public:
		WaitException() = default;
		WaitException(char const*);
	}; // class CGI::WaitException

	class CGI::SocketException: public CGI::Exception {
	public:
		SocketException() = default;
		SocketException(char const*);
	}; // class CGI::SocketException

	class CGI::ForkException: public CGI::Exception {
	public:
		ForkException() = default;
		ForkException(char const*);
	}; // class CGI::ForkException

	enum class CGI::ProcessStatus {
		busy,
		success,
		failure,
		aborted,
	}; // enum class CGI::Status
}; // namespace job

#endif // JOB_CGI_HPP
