#include "job/job.hpp"
#include "job/CGI.hpp"

#include <cerrno>
#include <cstring>

using job::BaseResource;
using job::CGI;

extern int	errno;

BaseResource::Exception::Exception():
	_msg(::strerror()) {}

BaseResource::Exception::Exception(char const* msg):
	_msg(msg) {}

char const*
BaseResource::Exception::what() const noexcept {
	return (_msg);
}

BaseResource::IOException::IOException(char const* msg):
	BaseResource::Exception(msg) {}
	
CGI::WaitException::WaitException(char const* msg):
	BaseResource::Exception(msg) {}
	
CGI::SocketException::SocketException(char const* msg):
	BaseResource::Exception(msg) {}

CGI::ForkException::ForkException(char const* msg):
	BaseResource::Exception(msg) {}
