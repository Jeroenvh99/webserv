#include "CGI.hpp"

#include <unistd.h>

CGI::CGI(Worker::Job const& job):
	_pid(_no_child),
	_socket() {
	_fork(job);
}

CGI::~CGI() {
	kill();
}
