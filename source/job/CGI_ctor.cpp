#include "job/CGI.hpp"

#include <unistd.h>

using job::CGI;

CGI::CGI(Job const& job):
	_pid(_no_child),
	_socket() {
	_fork(job);
}

CGI::~CGI() {
	kill();
}
