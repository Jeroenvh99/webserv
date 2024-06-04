#include "CGI.hpp"

#include <cerrno>
#include <cstring>

extern int	errno;

CGI::PipeException::PipeException():
	std::runtime_error(strerror(errno)) {}
	
CGI::PipeException::PipeException(char const* msg):
	std::runtime_error(msg) {}

CGI::ForkException::ForkException():
	std::runtime_error(strerror(errno)) {}

CGI::ForkException::ForkException(char const* msg):
	std::runtime_error(msg) {}
