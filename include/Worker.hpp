#ifndef WORKER_HPP
# define WORKER_HPP

# include "webserv.hpp"

# include "Buffer.hpp"
# include "Environment.hpp"
# include "http/Request.hpp"
# include "http/Response.hpp"
# include "job/Resource.hpp"
# include "job/CGI.hpp"
# include "time.hpp"

# include <iostream>
# include <optional>

class Worker {
public:
	enum class State;
	enum class InputStatus;
	enum class OutputStatus;

	static constexpr double	timeout_interval = 10.0; // s

	Worker();
	~Worker();
	Worker(Worker&) = delete;
	Worker(Worker&&);
	Worker&	operator=(Worker const&) = delete;
	Worker&	operator=(Worker&&);

	std::optional<http::Status>	start(job::Job const&);
	void						start(job::RedirectionJob const& job);
	void						start(job::ErrorJob const&);
	void						stop() noexcept;

	OutputStatus	fetch(webserv::Buffer&);
	InputStatus		deliver(webserv::Buffer const&);

private:
	size_t	read(webserv::Buffer&);
	size_t	write(webserv::Buffer const&);
	bool	timeout() const noexcept;

	State				_state;
	webserv::Time		_last_read;
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

enum class Worker::InputStatus {
	pending,
	failure,
}; // enum class Worker::InputStatus

enum class Worker::OutputStatus {
	pending,
	success,
	failure,
	aborted,
	timeout,
}; // enum class Worker::OutputStatus

#endif // WORKER_HPP