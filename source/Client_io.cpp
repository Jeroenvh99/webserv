#include "Client.hpp"

bool
Client::parse_request(webserv::Buffer& buf) {
	buf.put(_impl._buffer); // catch error?
	if (_impl._parser.parse(_impl._buffer, _impl._request) == http::Parser::State::done) {
		_impl._request_body = _impl._request.expects_body();
		if (_impl._request_body.type() == http::Body::Type::invalid)
			throw (http::Parser::Exception("bad body description"));
		_impl._buffer_flush(buf);
		return (true);
	}
	return (false);
}

bool
Client::parse_response(webserv::Buffer const& buf) {
	_impl._buffer.clear();
	buf.put(_impl._buffer);

	auto	header = http::parse_header_cgi(_impl._buffer);

	while (header) {
		if (header->first == "") { // blank line was processed; end of headers
			_impl._response.init_from_headers();
			_impl._response_body = _impl._response.expects_body(); // replace by:
			// if response specifies Content-Length, use that value, injecting an error if it is exceeded
			// else if response specifies Transfer-Encoding = chunked, assume CGI output is already chunked
			// else, the server enchunks the response data itself

			std::string	trail; // extract leftovers

			_impl._buffer_flush(trail);
			_impl._buffer.clear();
			_impl._buffer << _impl._response << trail;
			_impl._state = State::work;
			return (true);
		}
		_impl._response.header_add(std::move(*header));
		header = http::parse_header_cgi(_impl._buffer);
	}
	return (false);
}

job::Status
Client::respond(job::Job const& job) {
	std::optional<http::Status>	jstat = _impl._worker.start(job);

	if (!jstat)						 // job is CGI; must be waited for first
		_impl._state = State::parse_response;
	else if (http::is_error(*jstat)) // job couldn't be started
		return (respond({*jstat, job}));
	else {
		_impl._response = http::Response(*jstat);
		// todo: insert more headers based on file type
		_impl._buffer_empty();
		_impl._buffer.clear();
		_impl._buffer << _impl._response;
		_impl._state = State::work;
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
	_impl._state = State::work;
	return (wait());
}

job::Status
Client::deliver(webserv::Buffer const& buf) {
	if (_impl._request_body.type() == http::Body::Type::none)
		return (job::Status::failure); // will not accept unexpected body // move this check elsewhere?
	// todo: dechunk if necessary
	_impl._worker.write(buf);

	return (wait());
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
		_impl._state = State::idle;
	return (stat);
}