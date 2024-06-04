#ifndef CGI_HPP
# define CGI_HPP

#include <stdexcept>
#include <string>
#include <vector>

#include "http/Request.hpp"

class CGI {
public:
	class Exception;
	class PipeException;
	class ForkException;

	CGI() = delete;
	~CGI();
	CGI(CGI const&) = delete;
	CGI(CGI&&);
	CGI(http::Request const&);

	void	launch(http::Request const&);
	size_t	read() const;
	void	kill();

private:
	friend int main(int, char**, char**);

	using fd = int;

	static void	_redirect_stdout(fd);

	void	_fork(http::Request const&);
	void	_exec(http::Request const&);

	//char**	_new_argv(http::Request const&) const;
	std::vector<std::string>	_get_envv(http::Request const&) const;

	static char**				_envp;
	static size_t				_envsize;
	static constexpr pid_t		_no_child = 0;
	static constexpr size_t		_read_end = 0;
	static constexpr size_t		_write_end = 1;
	static constexpr size_t		_additional_vars = 5;

	pid_t	_pid;
	fd		_ifd;
}; // class CGI

class CGI::Exception: public std::exception {};

class CGI::PipeException: public CGI::Exception, std::runtime_error {
public:
	PipeException();
	PipeException(char const*);
}; // class CGI::PipeException

class CGI::ForkException: public CGI::Exception, std::runtime_error {
public:
	ForkException();
	ForkException(char const*);
}; // class CGI::ForkException

#endif // CGI_HPP
