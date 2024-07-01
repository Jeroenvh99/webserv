#include "Worker.hpp"

Worker::Worker():
	_state(State::done) {}

Worker::~Worker() {
	stop();
}

Worker::Worker(Worker&& that):
	_state(that._state) {
	switch (_state) {
	case State::resource:
		new (&_resource) job::Resource(std::move(that._resource));
		break;
	case State::cgi:
		new (&_cgi) job::CGI(std::move(that._cgi));
		break;
	case State::done: break;
	}
}

Worker&
Worker::operator=(Worker&& that) {
	if (this == &that)
		return (*this);
	stop();
	_state = that._state;
	switch (_state) {
	case State::resource:
		new (&_resource) job::Resource(std::move(that._resource));
		break;
	case State::cgi:
		new (&_cgi) job::CGI(std::move(that._cgi));
		break;
	case State::done: break;
	}
	return (*this);
}