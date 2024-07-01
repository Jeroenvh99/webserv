#include "job/CGI.hpp"

#include <cerrno>
#include <cstring>

using job::CGI;

extern int	errno;

CGI::WaitException::WaitException():
	std::runtime_error(strerror(errno)) {}
	
CGI::WaitException::WaitException(char const* msg):
	std::runtime_error(msg) {}

CGI::SocketException::SocketException():
	std::runtime_error(strerror(errno)) {}
	
CGI::SocketException::SocketException(char const* msg):
	std::runtime_error(msg) {}

CGI::ForkException::ForkException():
	std::runtime_error(strerror(errno)) {}

CGI::ForkException::ForkException(char const* msg):
	std::runtime_error(msg) {}
