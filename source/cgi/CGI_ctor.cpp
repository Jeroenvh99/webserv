#include "CGI.hpp"

#include <unistd.h>

CGI::CGI(std::initializer_list<std::string> ilist):
	_pid(_no_child),
	_ifd(-1),
	_ofd(-1),
	_env{ilist} {}

CGI::~CGI() {
	kill();
	if (_ifd > 2)
		::close(_ifd);
	if (_ofd > 2)
		::close(_ofd);
}
