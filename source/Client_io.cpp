#include "Client.hpp"

bool
Client::parse_request(webserv::Buffer& buf) {
	_impl._buffer_fill(buf);
	if (_impl._parser.parse(_impl._buffer, _impl._request) == http::parse::RequestParser::State::done) {
		http::Body const	body = _impl._request.expects_body();

		switch (body.type()) {
		case http::Body::Type::none:
			_impl._istate = InputState::closed; // fetch function will set it to parse_request when there's nothing left to fetch
			return (true);
		case http::Body::Type::by_length:
			_impl._istate = InputState::deliver;
			_impl._body_size = body.length();
			break;
		case http::Body::Type::chunked:
			_impl._istate = InputState::dechunk;
			break;
		}
		_impl._buffer_flush(buf);
		return (true);
	}
	return (false);
}

bool
Client::parse_response(webserv::Buffer const& buf) {
	_impl._buffer.clear();
	buf.put(_impl._buffer);

	std::string	line;

	std::getline(_impl._buffer, line);
	while (!_impl._buffer.eof()) {
		if (line.length() == 0) { // blank line was processed; end of headers
			_impl._response.init_from_headers();
			_impl._response_body = _impl._response.expects_body(); // replace by:
			// if response specifies Content-Length, use that value, injecting an error if it is exceeded
			// else if response specifies Transfer-Encoding = chunked, assume CGI output is already chunked
			// else, the server enchunks the response data itself

			std::string	trail; // extract leftovers

			_impl._buffer_flush(trail);
			_impl._buffer.clear();
			_impl._buffer << _impl._response << trail;
			_impl._ostate = OutputState::fetch;
			return (true);
		}
		_impl._response.headers().insert(line);
		std::getline(_impl._buffer, line);
	}
	_impl._buffer << line; // eof was reached
	return (false);
}

void
Client::respond(job::Job const& job) {
	int httpredirindex = job::is_httpredirect(job);
	if (httpredirindex > -1){
		job::RedirectionJob redirjob(job, httpredirindex);
		respond(redirjob);
		if (redirjob.permanent)
			throw (Client::RedirectionException(http::Status::moved_permanently));
		else
			throw (Client::RedirectionException(http::Status::found));
	}
	std::optional<http::Status>	jstat = _impl._worker.start(job);

	if (!jstat)						 // job is CGI; must be waited for first
		_impl._ostate = OutputState::parse_response;
	else if (http::is_error(*jstat)) { // job couldn't be started
		respond({*jstat, job});
		throw (Client::HTTPErrorException(*jstat));
	} else {
		_impl._response = http::Response(*jstat);
		_impl._buffer_empty();
		_impl._buffer.clear();
		_impl._buffer << _impl._response;
		_impl._ostate = OutputState::fetch;
	}
}

void
Client::respond(job::RedirectionJob const& job) {
	if (job.permanent)
		_impl._response = http::Response(http::Status::moved_permanently);
	else
		_impl._response = http::Response(http::Status::found);
	_impl._buffer_empty();
	_impl._buffer.clear();
	_impl._buffer << _impl._response; // response line and headers
	_impl._worker.start(job);
	_impl._ostate = OutputState::fetch;
}

void
Client::respond(job::ErrorJob const& job) {
	_impl._response = http::Response(job.status);
	_impl._buffer_empty();
	_impl._buffer.clear();
	_impl._buffer << _impl._response; // response line and headers
	_impl._worker.start(job);
	_impl._ostate = OutputState::fetch;
}

Client::OperationStatus
Client::deliver(webserv::Buffer const& buf) {
	if (buf.len() > _impl._body_size) // actual body size > Content-Length
		return (OperationStatus::failure);
	_impl._body_size -= buf.len();
	switch (_impl._worker.deliver(buf)) {
	case Worker::InputStatus::pending:
		if (_impl._body_size == 0) {
			if (_impl._ostate == OutputState::closed)
				_impl._clear();
			else
				_impl._istate = InputState::closed;
			return (OperationStatus::success);
		}
		return (OperationStatus::pending);
	default: // failure
		return (OperationStatus::failure);
	}
}

Client::OperationStatus
Client::dechunk_and_deliver(webserv::Buffer&) {
	// try {
	// 	_impl._dechunker.dechunk(buf);
	// } catch (http)
	// switch (_impl._worker.deliver(buf)) {
	// case Worker::InputStatus::pending:
	// 	if (_impl._dechunker.done()) {
	// 		_impl._istate = (_impl._ostate == OutputState::closed)
	// 			? InputState::parse_request
	// 			: InputState::closed;
	// 		return (OperationStatus::success);
	// 	}
	// 	return (OperationStatus::pending);
	// default: // failure
		return (OperationStatus::failure);
	// }
}

Client::OperationStatus
Client::fetch(webserv::Buffer& buf) {
	if (!_impl._buffer.eof()) { // empty client buffer before fetching from worker
		buf.get(_impl._buffer);
		return (OperationStatus::pending);
	}
	switch (_impl._worker.fetch(buf)) {
	case Worker::OutputStatus::success:
		_impl._ostate = OutputState::closed;
		if (_impl._istate == InputState::closed)
			_impl._clear();
		return (OperationStatus::success);
	case Worker::OutputStatus::pending:
		return (OperationStatus::pending);
	case Worker::OutputStatus::timeout:
		return (OperationStatus::timeout);
	default: // failure, aborted
		return (OperationStatus::failure);
	}
}