#include "network/Handle.hpp"

extern "C" {
#include <unistd.h>
}

using network::Handle;

// Basic operations

Handle::Handle():
	_raw(_invalid_handle) {}

Handle::~Handle() {
	close();
}

Handle::Handle(Raw raw):
	_raw(raw) {}


Handle::Handle(Handle const& that):
	_raw(dup(that._raw)) {
	if (!good()) {
		throw Exception("error creating handle");
	}
}

Handle::Handle(Handle&& that) noexcept:
	_raw(that.release()) {}


Handle&
Handle::operator=(Handle const& that) {
	if (this == &that) {
		return (*this);
	}
	if (!that.good()) {
		close();
		return (*this);
	}
	Raw const duplicate = dup(that._raw);
	if (duplicate == _invalid_handle) {
		throw Exception("error creating handle");
	}
	close();
	_raw = duplicate;
	return (*this);
}

Handle&
Handle::operator=(Handle&& that) noexcept {
	if (this == &that) {
		return (*this);
	}
	close();
	_raw = that.release();
	return (*this);
}

// Public methods

bool
Handle::good() const noexcept {
	return (_raw != _invalid_handle);
}

void
Handle::close() {
	if (good() && ::close(release()) == -1) {
		throw (Exception("close"));
	}
}

// Protected methods

bool
Handle::operator==(Raw const& that) const noexcept {
	return (_raw == that);
}

bool
Handle::operator!=(Raw const& that) const noexcept {
	return (_raw != that);
}

Handle::Raw
Handle::raw() const noexcept {
	return (_raw);
}

// Private methods

Handle::Raw
Handle::release() noexcept {
	Raw const	to_release = _raw;

	_raw = _invalid_handle;
	return (to_release);
}
