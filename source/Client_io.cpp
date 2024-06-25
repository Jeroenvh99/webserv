#include "Client.hpp"

http::Request::Parser::State
Client::parse(webserv::Buffer& buf) {
	buf.write(_impl._buffer); // catch error?
	if (_impl._parser.parse(_impl._buffer, _impl._request) == http::Request::Parser::State::done) {
		_impl._state = State::work;
		flush(buffer);
	}
	return (_impl._parser.state());
}

job::StatusOption
Client::respond(job::Job const& job) {
	_impl._state = State::work;
	return (_impl._worker.start(job));
}

void
Client::respond(job::ErrorJob const& job) {
	_impl._state = State::work;
	_impl._buffer.clear();
	_impl._buffer << std::string(http::Response(job.status)); // response line and headers
	_impl._worker.start(job); // 
}

job::StatusOption
Client::deliver(webserv::Buffer const& buf) {
	_impl._worker.read(buf);
	return (_impl._worker.wait());
}

job::StatusOption
Client::fetch_body(webserv::Buffer& buf) {
	_impl._worker.write(buf);
	return (_impl._worker.wait());
}

size_t
Client::flush(webserv::Buffer& buf) {
	size_t const	bytes = buf.read(_impl._buffer);

	_impl._buffer.clear();
	return (bytes);
}