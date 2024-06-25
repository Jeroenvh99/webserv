#include "CGI.hpp"
#include "Environment.hpp"

#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

using route::Location;

static void	_redirect_io(Socket&);
static int	_dup2(int, int);

// Public methods

void
CGI::kill() {
	if (_pid == _no_child)
		return;
	::kill(_pid, SIGKILL);
	_pid = _no_child;
}

CGI::ExitStatus
CGG::wait() {
	int status;

	switch (waitpid(_pid, &status, WNOHANG)) {
	case -1:
		throw (WaitException());
	case 0:
		return (ExitStatus::working);
	default:
		_pid = _no_child;
		if (WIFEXITED(status))
			return (WEXITSTATUS == 0
				? ExitStatus::success
				: ExitStatus::failure);
		return ExitStatus::terminated;
	}
}

// Private methods

void
CGI::_fork(Job const& job) {
	using SocketPair = network::SocketPair<
		network::Domain::local,
		network::Type::stream>;

	SocketPair	pair;

	_pid = ::fork();
	if (_pid == -1)
		throw (ForkException());
	if (_pid == _no_child)
		_socket = std::move(pair.first);
	else {
		pair.first.close();
		_redirect_io(pair.second);
		_exec(job);
	}
}

void
CGI::_exec(Job const& job) {
	std::string		pathname(job.location.to());
	char* const		cpathname = pathname.data();
	char* const		cargv[2] = {cpathname, nullptr};

	job.environment.build();
	if (::execve(cpathname, cargv, job.environment.cenv()) == -1) {
		::perror("execve");
		::exit(EXIT_FAILURE);
	}
}

// Helpers

static void
_redirect_io(Socket& socket) {
	Socket::Raw const	fd = socket.release();

	_dup2(fd, STDIN_FILENO);
	_dup2(fd, STDOUT_FILENO);
	_dup2(fd, STDERR_FILENO);
	::close(fd);
}

static int
_dup2(int from, int to) {
	if (::_dup2(from, to) == -1) {
		::perror("dup2");
		::exit(EXIT_FAILURE);
	}
}