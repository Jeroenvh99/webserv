#include "CGI.hpp"

static char**		_make_cstrv(std::vector<std::string>&);

// Public methods

void
CGI::launch(http::Request const& req) {
	int status;
	// if (this->bodyLength > 0)
	//     fcntl(pipefds[WRITE_END], F_SETPIPE_SZ, this->bodyLength);

	// write(pipefds[WRITE_END], this->request.data(), this->bodyLength);
	_fork(req);
	waitpid(_pid, &status, 0); // replace to enable asynchronous CGI
	std::cout << std::to_string(status) << std::endl;
}

void
CGI::kill() {
	if (_pid == _no_child)
		return;
	::kill(_pid, SIGKILL);
	_pid = _no_child;
}

// Private methods

void
CGI::_fork(http::Request const& req) {
	fd	pipefd[2];

	if (::pipe(pipefd) == -1)
		throw (PipeException());
	_pid = ::fork();
	if (_pid == -1)
		throw (ForkException());
	if (_pid == _no_child) {
		::close(pipefd[_write_end]);
		_ifd = pipefd[_read_end];
	} else {
		_redirect_stdout(pipefd[_write_end]);
		_exec(req);
	}
}

void
CGI::_exec(http::Request const& req) {
	std::string					pathname = req.uri().path();
	std::vector<std::string>	envv = _get_envv(req);

	char* const		cpathname = pathname.data();
	char* const		cargv[2] = {cpathname, nullptr};
	char** const	cenvp = _make_cstrv(envv);

	if (::execve(cpathname, cargv, cenvp) == -1) {
		::perror("execve");
		::exit(EXIT_FAILURE);
	}
}

void
CGI::_redirect_stdout(fd ofd) {
	::dup2(ofd, STDOUT_FILENO);
	::close(ofd);
}

std::vector<std::string>
CGI::_get_envv(http::Request const& req) const {
	std::vector<std::string>	envv;

	envv.reserve(_envsize + 5); // Reserve enough space for _envv_append()
	for (size_t i = 0; i < _envsize; ++i)
		if (_envp[i])
			envv.push_back(_envp[i]);
	_envv_append(envv, req);
	return (envv);
}

// Non-member helpers

static char**
_make_cstrv(std::vector<std::string>& vec) {
	char**	cstrv = new char*[vec.size() + 1];

	for (size_t i = 0; i < vec.size(); ++i)
		cstrv[i] = vec[i].data();
	cstrv[vec.size()] = nullptr;
	return (cstrv);
}
