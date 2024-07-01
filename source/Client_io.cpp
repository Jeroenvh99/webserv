#include "Client.hpp"

bool
Client::parse_request(webserv::Buffer& buf) {
	buf.put(_impl._buffer); // catch error?
	if (_impl._parser.parse(_impl._buffer, _impl._request) == http::Parser::State::done) {
		_impl._request_body = _impl._request.expects_body();
		if (_impl._request_body.type() == http::Body::Type::invalid)
			throw (http::Parser::Exception("bad body description"));
		flush(buf);
		return (true);
	}
	return (false);
}

bool
Client::parse_response(webserv::Buffer const& buf) {
	_impl._buffer << buf;

	auto	header = http::parse_header_cgi(_impl._buffer);

	while (header) {
		if (header->first == "") { // blank line was processed; end of headers
			_impl._response.init_from_headers();
			_impl._response_body = _impl._response.expects_body();

			std::string	trail(_impl._buffer.str()); // extract leftovers

			_impl._reset_buffer();
			_impl._buffer << _impl._response << trail;
			_impl._state = State::work;
			return (true);
		}
		_impl._response.header_add(std::move(*header));
		header = http::parse_header_cgi(_impl._buffer);
	}
	return (false);
}

job::StatusOption
Client::respond(job::Job const& job) {
	job::StatusOption	status = _impl._worker.start(job);

	_impl._state = _impl._worker.state() == Worker::State::cgi
		? State::parse_response
		: State::work;
	return (status);
}

void
Client::respond(job::ErrorJob const& job) {
	_impl._response = http::Response(job.status);
	_impl._reset_buffer();
	_impl._buffer << _impl._response; // response line and headers
	_impl._worker.start(job);
	_impl._state = State::work;
}

job::StatusOption
Client::deliver(webserv::Buffer const& buf) {
	if (_impl._request_body.type() == http::Body::Type::none)
		return (http::Status::bad_request); // will not accept unexpected body
	_impl._worker.write(buf);
	return (_impl._worker.wait());
}

job::StatusOption
Client::fetch(webserv::Buffer& buf) {
	if (!_impl._buffer.eof()) { // empty client buffer before fetching from worker
		_impl._buffer >> buf;
		return (std::nullopt);
	}
	_impl._worker.read(buf);
	return (_impl._worker.wait());
}

size_t
Client::flush(webserv::Buffer& buf) {
	size_t const	bytes = buf.get(_impl._buffer);

	_impl._reset_buffer();
	return (bytes);
}