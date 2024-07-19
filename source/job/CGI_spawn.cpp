#include "job/CGI.hpp"
#include "Environment.hpp"

#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

using job::CGI;
using route::Location;

static void	_dup2(int, int);

// Public methods

void
CGI::kill() noexcept {
	if (_pid == _no_child)
		return;
	::kill(_pid, SIGKILL);
	_pid = _no_child;
}

job::Status
CGI::wait() {
	if (_pid == _no_child)
		return (Status::success);

	int wstat;

	switch (waitpid(_pid, &wstat, WNOHANG)) {
	case -1:
		throw (WaitException());
	case 0:
		return (Status::pending);
	default:
		_pid = _no_child;
		if (WIFSIGNALED(wstat))
			return (Status::aborted);
		if (WEXITSTATUS(wstat) != EXIT_SUCCESS)
			return (Status::failure);
		return (Status::success);
	}
}

// Private methods

void
CGI::_fork(Job const& job) {
	network::SocketPair<Socket>	pair;

	_pid = ::fork();
	switch (_pid) {
	case -1:
		throw (ForkException());
	case _no_child:
		pair.first.close();
		_redirect_io(pair.second);
		_exec(job);
		break;
	default:
		_socket = std::move(pair.first);
		break;
	}
}

void
CGI::_redirect_io(Socket& socket) {
	auto const	fd = socket.release();

	_dup2(fd, STDIN_FILENO);
	_dup2(fd, STDOUT_FILENO);
	_dup2(fd, STDERR_FILENO);
	::close(fd);
}

void
CGI::_exec(Job const& job) {
	std::string		pathname(job.location.to());
	Environment		env(job.environment);
	char* const		cpathname = pathname.data();
	char* const		cargv[2] = {cpathname, nullptr};

	env.build();
	if (::execve(cpathname, cargv, env.cenv()) == -1) {
		::perror("execve");
		::exit(EXIT_FAILURE);
	}
}

// Helpers

static void
_dup2(int from, int to) {
	if (::dup2(from, to) == -1) {
		::perror("dup2");
		::exit(EXIT_FAILURE);
	}
}