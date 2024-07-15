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
	enum class State;

	Worker();
	~Worker();
	Worker(Worker&) = delete;
	Worker(Worker&&);
	Worker&	operator=(Worker const&) = delete;
	Worker&	operator=(Worker&&);

	std::optional<http::Status>	start(job::Job const&);
	void						start(job::ErrorJob const&);
	void						stop() noexcept;
	job::Status					wait();

	size_t	read(webserv::Buffer&);
	size_t	write(webserv::Buffer const&);

	State	state() const noexcept; // remove

private:
	State				_state;
	union {
		job::Resource	_resource;
		job::CGI		_cgi;
	}; // anonymous union
}; // class Worker

enum class Worker::State {
	none,
	resource,
	cgi,
}; // enum class Worker::State

#endif // WORKER_HPP