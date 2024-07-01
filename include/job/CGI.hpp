#ifndef JOB_CGI_HPP
# define JOB_CGI_HPP

#include "job.hpp"
#include "Buffer.hpp"
#include "network/StreamSocket.hpp"

#include <iostream>
#include <stdexcept>

namespace job {
	class CGI {
	public:
		class Exception;
		class WaitException;
		class SocketException;
		class ForkException;

		CGI(Job const&);
		~CGI();

		size_t		write(webserv::Buffer const&) const;
		size_t		read(webserv::Buffer&) const;
		ExitOption	wait();
		void		kill();

	private:
		using Socket = network::StreamSocket<network::Domain::local>;

		void	_fork(Job const&);
		void	_redirect_io(Socket&);
		void	_exec(Job const&);

		static constexpr pid_t	_no_child = 0;

		pid_t	_pid;
		Socket	_socket;
	}; // class CGI

	class CGI::Exception: public std::exception {};

	class CGI::WaitException: public CGI::Exception, std::runtime_error {
	public:
		WaitException();
		WaitException(char const*);
	}; // class CGI::WaitException

	class CGI::SocketException: public CGI::Exception, std::runtime_error {
	public:
		SocketException();
		SocketException(char const*);
	}; // class CGI::PipeException

	class CGI::ForkException: public CGI::Exception, std::runtime_error {
	public:
		ForkException();
		ForkException(char const*);
	}; // class CGI::ForkException
}; // namespace job

#endif // JOB_CGI_HPP
