#include "CGI.hpp"

#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

static void		_envv_append(std::vector<std::string>&, http::Request const&);
static char**	_make_cstrv(std::vector<std::string>&);

char**	CGI::_envp;
size_t	CGI::_envsize;

void
CGI::launch(http::Request const& req) {
	int status;
	// if (this->bodyLength > 0)
	//     fcntl(pipefds[WRITE_END], F_SETPIPE_SZ, this->bodyLength);

	// write(pipefds[WRITE_END], this->request.data(), this->bodyLength);
	_fork(req);
	waitpid(_pid, &status, 0); // replace to enable asynchronous CGI
}

void
CGI::_fork(http::Request const& req) {
	fd	pipefd[2];

	if (pipe(pipefd) == -1)
		throw (PipeException());
	_pid = fork();
	if (_pid == -1)
		throw (ForkException());
	if (_pid == _no_child) {
		close(pipefd[_write_end]);
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

	if (execve(cpathname, cargv, cenvp) == -1) {
		perror("execve: ");
		exit(EXIT_FAILURE);
	}
}

void
CGI::_redirect_stdout(fd ofd) {
	dup2(ofd, STDOUT_FILENO);
	close(ofd);
}

std::vector<std::string>
CGI::_get_envv(http::Request const& req) const {
	std::vector<std::string>	envv;

	envv.reserve(_envsize + _additional_vars);
	for (size_t i = 0; i < _envsize; ++i)
		envv.push_back(_envp[i]);
	_envv_append(envv, req);
	return (envv);
}

static void
_envv_append(std::vector<std::string>& envv, http::Request const& req) {
	envv.push_back("REDIRECT_STATUS=200");
	envv.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envv.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envv.push_back("SERVER_SOFTWARE=Webserv");
	if (req.has_header("Content-Type"))
		envv.push_back(std::string("CONTENT_TYPE") + "+" + req.header("Content-Type"));
}
	
static char**
_make_cstrv(std::vector<std::string>& vec) {
	char**	cstrv = new char*[vec.size() + 1];

	for (size_t i = 0; i < vec.size(); ++i)
		cstrv[i] = vec[i].data();
	cstrv[vec.size()] = nullptr;
	return (cstrv);
}
