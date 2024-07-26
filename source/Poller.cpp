#include "Poller.hpp"

using webserv::Poller;

Poller	g_poller;

// Accessors

Poller::Events::iterator
Poller::begin() noexcept {
	return (_events.begin());
}

Poller::Events::iterator
Poller::end() noexcept {
	return (_events.end());
}

Poller::Event
Poller::event(network::SharedHandle const& handle) const {
	return (_events.at(handle));
}

// Modifiers

void
Poller::clear_events() noexcept {
	_events.clear();
}

void
Poller::wait() {
	_events = network::Poller::wait<max_events>(timeout);
}