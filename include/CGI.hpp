#ifndef CGI_HPP
# define CGI_HPP

#include "webserv.hpp"
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>

#include "route.hpp"

class CGI {
public:
	class Exception;
	class PipeException;
	class ForkException;

	CGI();
	~CGI();

	void	launch(route::Location const&, Client const&, Environment const&);
	size_t	write(std::ostream&) const;
	size_t	read(std::istream&) const;
	void	kill();

private:
	using fd = int;

	static void	_redirect_stdout(fd);

	void	_fork(route::Location const&, Client const&, Environment const&);
	void	_exec(route::Location const&, Client const&, Environment const&);

	static constexpr pid_t		_no_child = 0;
	static constexpr size_t		_read_end = 0;
	static constexpr size_t		_write_end = 1;

	pid_t		_pid;
	fd			_ifd;
	fd			_ofd;
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
