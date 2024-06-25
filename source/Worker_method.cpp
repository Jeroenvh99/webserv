#include "Worker.hpp"

static job::StatusOption	_to_status_option(int);

job::StatusOption
Worker::start(Job const& job) {
	stop();
	if (!job.location.allows_method(loc.request.method()))
		return (http::Status::method_not_allowed);
	_state = job.is_cgi() ? State::cgi : State::resource;
	switch (_state) {
	case State::resource:
		new (&_resource) Resource(job);
		return (_resource.status());
	case State::cgi:
		new (&_cgi) CGI(Job);
		return (std::nullopt_t);
	default: break;
	}
}

void
Worker::start(ErrorJob const& job) {
	stop();
	_state = State::resource;
	new (&_resource) Resource(job);
}

void
Worker::stop() noexcept {
	switch (_state) {
	case State::resource:
		_resource.~Resource();
		_state = State::done;
		break;
	case State::cgi:
		_cgi.~CGI();
		_state = State::done;
		break;
	default: break;
	}
}

StatusOption
Worker::wait() {
	switch (_state) {
	case State::resource:
		StatusOption const	status = _resource.status();

		if (status)
			stop();
		return (status);
	case State::cgi:
		ExitOption const	exit = _cgi.wait();

		if (exit) 
			return (stop(), _to_status_option(*exit))
		return (std::nullopt_t);
	case State::done:
		return (http::Status::ok);
	}
}

static StatusOption
_to_status_option(int exit) {
	if (WIFSIGNALED(exit) || WEXITSTATUS(exit) != EXIT_SUCCESS)
		return (http::Status::internal_error);
	return (http::Status::ok); // actual status must be derived from header
}