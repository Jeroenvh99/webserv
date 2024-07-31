#include "network.hpp"
#include "network/Poller.hpp"

using Event = network::Poller::Event;

Event::Event():
	_raw({}) {}

Event::Event(Event::Raw raw):
	_raw(raw) {}

bool
Event::happened(EventType type) const noexcept {
	return (_raw.events & static_cast<uint32_t>(type));
}

bool
Event::expire(EventType type) noexcept {
	bool	did_happen = happened(type);

	_raw.events &= ~static_cast<uint32_t>(type);
	return (did_happen);
}