#include "Worker.hpp"

Worker::Worker():
	_state(State::none),
	_bytes_delivered(0),
	_bytes_delivered_max(0),
	_bytes_fetched(0) {}

Worker::~Worker() {
	stop();
}

Worker::Worker(Worker&& that):
	_state(that._state),
	_last_read(that._last_read),
	_bytes_delivered(that._bytes_delivered),
	_bytes_delivered_max(that._bytes_delivered_max),
	_bytes_fetched(that._bytes_fetched) {
	switch (_state) {
	case State::resource:
		new (&_resource) job::Resource(std::move(that._resource));
		break;
	case State::multipart_resource:
		new (&_multipart_resource) job::MultipartResource(std::move(that._multipart_resource));
	case State::cgi:
		new (&_cgi) job::CGI(std::move(that._cgi));
		break;
	case State::none: break;
	}
}

Worker&
Worker::operator=(Worker&& that) {
	if (this == &that)
		return (*this);
	stop();
	_state = that._state;
	_last_read = that._last_read;
	_bytes_delivered = that._bytes_delivered;
	_bytes_delivered_max = that._bytes_delivered_max;
	_bytes_fetched = that._bytes_fetched;
	switch (_state) {
	case State::resource:
		new (&_resource) job::Resource(std::move(that._resource));
		break;
	case State::multipart_resource:
		new (&_multipart_resource) job::MultipartResource(std::move(that._multipart_resource));
	case State::cgi:
		new (&_cgi) job::CGI(std::move(that._cgi));
		break;
	case State::none: break;
	}
	return (*this);
}