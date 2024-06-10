#include "CGI.hpp"

#include <unistd.h>

char**	CGI::_envp;
size_t	CGI::_envsize;

CGI::CGI():
	_pid(_no_child),
	_ifd(-1) {}

CGI::~CGI() {
	kill();
	if (_ifd > 2)
		::close(_ifd);
}