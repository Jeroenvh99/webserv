#include "CGI.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define WRITE_END 1
#define READ_END 0

using http::CGI;

CGI::CGI(const HttpRequest &request)
{
    initEnv(request);
    exec(request);
}

void CGI::exec(const HttpRequest &request)
{
    pid_t pid;

    int pipefds[2];
    if (pipe(pipefds) == -1)
        throw CGIException("pipe() failed");

    // if (this->bodyLength > 0)
    //     fcntl(pipefds[WRITE_END], F_SETPIPE_SZ, this->bodyLength);

    // write(pipefds[WRITE_END], this->request.data(), this->bodyLength);
    close(pipefds[WRITE_END]);

    pid = fork();
    if (pid == -1)
        throw CGIException("fork() failed");

    // child
    if (pid == 0) {
        dup2(pipefds[READ_END], STDIN_FILENO);
        close(pipefds[READ_END]);

        char **argv = new char*[2];
        argv[0] = strdup("/usr/bin/python3");
        argv[1] = NULL;
        char **envp = new char*[_env.size() + 1];
        size_t i = 0;
        for (auto it = _env.begin(); it != _env.end(); it++, i++) 
        {
            envp[i] = strdup((it->first + "=" + it->second).c_str());
        }
        if (execve(argv[0], argv, envp) == -1)
            throw CGIException("execve() failed");
        delete[] envp;
    } else {
        int status;
        close(pipefds[READ_END]);
        waitpid(pid, &status, 0);
    }
}

void CGI::initEnv(const HttpRequest &request)
{
    auto headers = request.getHeaders();

    _env["REDIRECT_STATUS"] = "200";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["CONTENT_TYPE"] = headers["Content-Type"];
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "Webserv";
    // TODO: Other env variables
}
