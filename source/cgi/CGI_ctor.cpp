#include "CGI.hpp"

#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

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