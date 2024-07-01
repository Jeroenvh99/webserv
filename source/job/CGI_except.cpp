#include "job/CGI.hpp"

#include <cerrno>
#include <cstring>

using job::CGI;

extern int	errno;

CGI::Exception::Exception(char const* msg):
	_what(msg) {}

char const*
CGI::Exception::what() const noexcept {
	return (_what);
}

CGI::WaitException::WaitException():
	CGI::Exception(strerror(errno)) {}
	
CGI::WaitException::WaitException(char const* msg):
	CGI::Exception(msg) {}

CGI::SocketException::SocketException():
	CGI::Exception(strerror(errno)) {}
	
CGI::SocketException::SocketException(char const* msg):
	CGI::Exception(msg) {}

CGI::ForkException::ForkException():
	CGI::Exception(strerror(errno)) {}

CGI::ForkException::ForkException(char const* msg):
	CGI::Exception(msg) {}
