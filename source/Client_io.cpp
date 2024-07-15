#include "Client.hpp"

bool
Client::parse_request(webserv::Buffer& buf) {
	_impl._buffer_fill(buf);
	if (_impl._parser.parse(_impl._buffer, _impl._request) == http::parse::RequestParser::State::done) {
		http::Body const	body = _impl._request.expects_body();

		switch (body.type()) {
		case http::Body::Type::none:
			_impl._istate = InputState::closed;
			break;
		case http::Body::Type::by_length:
			_impl._istate = InputState::deliver; // and set length indicator
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

			// http::Body const	body = _impl._response.expects_body();
			_impl._response_body = _impl._response.expects_body(); // replace by:
			// if response specifies Content-Length, use that value, injecting an error if it is exceeded
			// else if response specifies Transfer-Encoding = chunked, assume CGI output is already chunked
			// else, the server enchunks the response data itself

			std::string	trail; // extract leftovers

			_impl._buffer_flush(trail);
			_impl._buffer.clear();
			_impl._buffer << _impl._response << trail;
			_impl._ostate = OutputState::fetch; // replace
			return (true);
		}
		_impl._response.headers().insert(line);
		std::getline(_impl._buffer, line);
	}
	_impl._buffer << line; // eof was reached
	return (false);
}

job::Status
Client::respond(job::Job const& job) {
	std::optional<http::Status>	jstat = _impl._worker.start(job);

	if (!jstat)						 // job is CGI; must be waited for first
		_impl._ostate = OutputState::parse_response;
	else if (http::is_error(*jstat)) // job couldn't be started
		return (respond({*jstat, job}));
	else {
		_impl._response = http::Response(*jstat);
		// todo: insert more headers based on file type
		_impl._buffer_empty();
		_impl._buffer.clear();
		_impl._buffer << _impl._response;
		_impl._ostate = OutputState::fetch;
	}
	return (wait());
}

job::Status
Client::respond(job::ErrorJob const& job) {
	_impl._response = http::Response(job.status);
	// todo: insert more headers based on file type
	_impl._buffer_empty();
	_impl._buffer << _impl._response; // response line and headers
	_impl._worker.start(job);
	_impl._ostate = OutputState::fetch;
	return (wait());
}

job::Status
Client::deliver(webserv::Buffer const& buf) {
	_impl._worker.write(buf);

	return (wait()); // replace: job status should not depend on the same mechanism that determines end of worker output
}

job::Status
Client::dechunk(webserv::Buffer const&) {
	return (job::Status::failure); // placeholder
}

job::Status
Client::fetch(webserv::Buffer& buf) {
	if (!_impl._buffer.eof()) { // empty client buffer before fetching from worker
		buf.get(_impl._buffer);
		return (job::Status::pending);
	}
	_impl._worker.read(buf); // todo: implement timeout; if read == 0 for too long, mark client to be closed
	return (wait());
}

job::Status
Client::wait() {
	job::Status const	stat = _impl._worker.wait();

	if (stat != job::Status::pending)
		_impl._clear(); // this should only clear output-related variables
	return (stat);
}