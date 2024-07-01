#include "Worker.hpp"

static job::StatusOption	_to_status_option(int);

job::StatusOption
Worker::start(job::Job const& job) {
	stop();
	if (!job.location.allows_method(job.request.method()))
		return (http::Status::method_not_allowed);
	_state = job.is_cgi() ? State::cgi : State::resource;
	switch (_state) {
	case State::resource:
		new (&_resource) job::Resource(job);
		return (_resource.status());
	case State::cgi:
		new (&_cgi) job::CGI(job);
		return (std::nullopt);
	default:
		return (http::Status::ok); // unreachable
	}
}

void
Worker::start(job::ErrorJob const& job) {
	stop();
	_state = State::resource;
	new (&_resource) job::Resource(job);
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

job::StatusOption
Worker::wait() {
	switch (_state) {
	case State::resource: {
		job::StatusOption const	rstat = _resource.status();

		if (rstat)
			stop();
		return (rstat);
	}
	case State::cgi: {
		job::ExitOption const	estat = _cgi.wait();

		if (estat) {
			stop();
			return (_to_status_option(*estat));
		}
		return (std::nullopt);
	}
	default:
		return (http::Status::ok); // or nullopt?
	}
}

Worker::State
Worker::state() const noexcept {
	return (_state);
}

size_t
Worker::write(webserv::Buffer const& buf) {
	switch (_state) {
	case State::resource:
		return (_resource.write(buf));
	case State::cgi:
		return (_cgi.write(buf));
	default:
		return (0);
	}
}

size_t
Worker::read(webserv::Buffer& buf) {
	switch (_state) {
	case State::resource:
		return (_resource.read(buf));
	case State::cgi:
		return (_cgi.read(buf));
	default:
		return (0);
	}
}

static job::StatusOption
_to_status_option(int exit) {
	if (WIFSIGNALED(exit) || WEXITSTATUS(exit) != EXIT_SUCCESS)
		return (http::Status::internal_error);
	return (http::Status::ok); // actual status must be derived from header
}