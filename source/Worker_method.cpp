#include "Worker.hpp"
#include "job/CGI.hpp"
#include "Server.hpp"
#include "logging/logging.hpp"

using Elog = logging::ErrorLogger::Level;

// Initializer methods

std::optional<http::Status>
Worker::start(job::Job const& job) {
	stop();
	_bytes_delivered_max = job.vserver.max_body_size();
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
	_bytes_delivered = 0;
	_bytes_delivered_max = 0;
	_bytes_fetched = 0;
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
		size_t const	bytes = read(wsbuf);
		if (bytes == 0) {
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
			return (OutputStatus::success);
		}
		_bytes_fetched += bytes;
		_last_read = webserv::Time();
		return (OutputStatus::pending);
	} catch (job::BaseResource::IOException& e) {
		if (timeout()) {
			if (_bytes_fetched == 0)
				return (stop(), OutputStatus::timeout);
			return (stop(), OutputStatus::failure);
		}
		return (OutputStatus::pending);
	} catch (job::BaseResource::Exception& e) {
		logging::elog.log(Elog::error, e.what());
		return (stop(), OutputStatus::failure);
	}
}

Worker::InputStatus
Worker::deliver(webserv::Buffer const& wsbuf) {
	try {
		_bytes_delivered += wsbuf.len();
		if (_bytes_delivered > _bytes_delivered_max)
			throw (job::BaseResource::IOException("maximum configured body size was exceeded"));
		if (write(wsbuf) != wsbuf.len())
			throw (job::BaseResource::IOException("incomplete write"));
		return (InputStatus::pending);
	} catch (job::BaseResource::IOException& e) {
		logging::elog.log(Elog::error, "Error writing to file: ", e.what());
		// logging::elog.log(Elog::debug, "Buffer contents: ", wsbuf);
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
		return (_multipart_resource.read(wsbuf));
	case State::cgi:
		return (_cgi.read(wsbuf));
	default:
		return (0);
	}
}

// Other methods

bool
Worker::timeout() const noexcept {
	auto	now = webserv::Time();
	auto	diff = ::difftime(now, _last_read);
	return (diff >= timeout_interval);
	// return (::difftime(webserv::Time(), _last_read) >= timeout_interval);
}
