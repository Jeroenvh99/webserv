#include "Worker.hpp"

Worker::Worker():
	_state(State::done) {}

Worker::~Worker() {
	reset();
}

Worker::Worker(Worker&& that):
	_state(that._state) {
	switch (_state) : {
	case State::file:
		new (&_resource) Resource(std::move(that._resource));
		break;
	case State::cgi:
		new (&_cgi) CGI(std::move(that._cgi));
		break;
	case State::done: break;
	}
}

Worker&
Worker::operator=(Worker&& that) {
	if (this == &that)
		return (*this);
	reset();
	_state = that._state;
	switch (_state) {
	case State::file:
		new (&_resource) Resource(std::move(that._resource));
		break;
	case State::cgi:
		new (&_cgi) CGI(std::move(that._cgi));
		break;
	case State::done: break;
	}
	return (*this);
}