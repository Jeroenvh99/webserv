#include "Worker.hpp"
#include "job/CGI.hpp"

// Initializer methods

std::optional<http::Status>
Worker::start(job::Job const& job) {
	stop();
	if (!job.location.allows_method(job.request.method()))
		return (http::Status::method_not_allowed);
	if (job.is_cgi()) {
		_state = State::cgi;
		new (&_cgi) job::CGI(job);
		return (std::nullopt);
	}
	
	auto	multipart_boundary = http::parse::is_multipart(job.request);

	if (multipart_boundary) {
		_state = State::multipart_resource;
		new (&_multipart_resource) job::MultipartResource();
		return (_multipart_resource.open(job, *multipart_boundary));
	}
	_state = State::resource;
	new (&_resource) job::Resource(); // job is neither CGI nor multipart
	return (_resource.open(job));
}

void
Worker::start(job::ErrorJob const& job) {
	stop();
	_state = State::resource;
	new (&_resource) job::Resource();
	_resource.open(job);
}

void
Worker::start(job::RedirectionJob const& job) {
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
		_state = State::none;
		break;
	case State::multipart_resource:
		_multipart_resource.~MultipartResource();
		_state = State::none;
		break;
	case State::cgi:
		_cgi.~CGI();
		_state = State::none;
		break;
	default: break;
	}
}

// I/O methods

Worker::OutputStatus
Worker::fetch(webserv::Buffer& wsbuf) {
	using ProcessStatus = job::CGI::ProcessStatus;

	try {
		if (read(wsbuf) == 0) {
			if (_state == State::cgi) {
				switch (_cgi.wait()) {
				case ProcessStatus::busy: // CGI running with closed stdout
					return (stop(), OutputStatus::failure);
				case ProcessStatus::success:
					return (OutputStatus::success);
				default: // failure, aborted
					return (OutputStatus::failure);
				}
			}
			return (stop(), OutputStatus::success);
		}
		_last_read = webserv::Time();
		return (OutputStatus::pending);
	} catch (job::BaseResource::IOException& e) {
		if (timeout())
			return (stop(), OutputStatus::timeout);
		return (OutputStatus::pending);
	} catch (job::BaseResource::Exception& e) {
		std::cerr << e.what() << std::endl;
		return (stop(), OutputStatus::failure);
	}
}

Worker::InputStatus
Worker::deliver(webserv::Buffer const& wsbuf) {
	try {
		if (write(wsbuf) != wsbuf.len())
			throw (job::BaseResource::IOException("incomplete write"));
		return (InputStatus::pending);
	} catch (job::BaseResource::IOException& e) {
		std::cerr << e.what() << std::endl;
		return (stop(), InputStatus::failure);
	}
}

size_t
Worker::write(webserv::Buffer const& wsbuf) {
	switch (_state) {
	case State::resource:
		return (_resource.write(wsbuf));
	case State::multipart_resource:
		return (_multipart_resource.write(wsbuf));
	case State::cgi:
		return (_cgi.write(wsbuf));
	default:
		return (0);
	}
}

size_t
Worker::read(webserv::Buffer& wsbuf) {
	switch (_state) {
	case State::resource:
		return (_resource.read(wsbuf));
	case State::multipart_resource:
		return (_multipart_resource.write(wsbuf));
	case State::cgi:
		return (_cgi.read(wsbuf));
	default:
		return (0);
	}
}

// Other methods

bool
Worker::timeout() const noexcept {
	return (::difftime(webserv::Time(), _last_read) >= timeout_interval);
}
