#include "network.hpp"
#include "network/Handle.hpp"
#include "network/Poller.hpp"

using network::SharedHandle;
using network::Poller;
using Event = Poller::Event;

// Private basic operations

Event::Event(SharedHandle const& handle, Raw const& raw):
	_handle(handle),
	_raw(raw) {}

// Public methods

SharedHandle
Event::handle() const noexcept {
	return (_handle);
}

bool
Event::happened(EventType type) const noexcept {
	return (_raw.events & static_cast<uint32_t>(type));
}
