#include "CGI.hpp"

#include <unistd.h>

CGI::CGI():
	_pid(_no_child),
	_ifd(-1),
	_ofd(-1) {}

CGI::~CGI() {
	kill();
	if (_ifd > 2)
		::close(_ifd);
	if (_ofd > 2)
		::close(_ofd);
}
