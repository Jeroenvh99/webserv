#include "Worker.hpp"

std::optional<http::Status>
Worker::start(job::Job const& job) {
	stop();
	if (!job.location.allows_method(job.request.method()))
		return (http::Status::method_not_allowed);
	_state = job.is_cgi() ? State::cgi : State::resource;
	switch (_state) {
	case State::resource:
		new (&_resource) job::Resource();
		return (_resource.open(job));
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
	new (&_resource) job::Resource();
	_resource.open(job);
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

job::Status
Worker::wait() {
	switch (_state) {
	case State::resource:
		return (_resource.status());
	case State::cgi:
		return (_cgi.wait());
	default:
		return (job::Status::pending);
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