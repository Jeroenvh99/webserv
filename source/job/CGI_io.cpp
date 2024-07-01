#include "job/CGI.hpp"

using job::CGI;

size_t
CGI::read(webserv::Buffer& buf) const {
	return (_socket.read(buf));
}

size_t
CGI::write(webserv::Buffer const& buf) const {
	return (_socket.write(buf));
}