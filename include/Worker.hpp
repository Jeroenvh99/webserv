#ifndef WORKER_HPP
# define WORKER_HPP

# include "webserv.hpp"
# include "Buffer.hpp"
# include "job/Resource.hpp"
# include "job/CGI.hpp"
# include "Environment.hpp"
# include "http/Request.hpp"
# include "http/Response.hpp"

# include <iostream>

class Worker {
public:
	Worker();
	~Worker();
	Worker(Worker&) = delete;
	Worker(Worker&&);
	Worker&	operator=(Worker const&) = delete;
	Worker&	operator=(Worker&&);

	job::StatusOption	start(job::Job const&);
	void				start(job::ErrorJob const&);
	void				stop() noexcept;
	job::StatusOption	wait();

	size_t	read(webserv::Buffer const&);
	size_t	write(webserv::Buffer&);

private:
	enum class State;

	State				_state;
	union {
		job::Resource	_resource;
		job::CGI		_cgi;
	}; // anonymous union
}; // class Worker

enum class Worker::State {
	done,
	resource,
	cgi,
}; // enum class Worker::State

#endif // WORKER_HPP