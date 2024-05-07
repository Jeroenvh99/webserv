#include "network/Poller.hpp"

#include <algorithm>

#include "network/utils.tpp"

using network::Handle;
using network::SharedHandle;
using network::Poller;

extern int	errno;

// maybe more detailed exception handling, encapsulating errno

void
Poller::add_shared(SharedHandle const& handle, EventTypes events, Modes modes) {
	Event::Raw	rev;

	rev.events = _get_bitmask(events) | _get_bitmask(modes);
	rev.data.fd = handle->raw();

	if (epoll_ctl(raw(), EPOLL_CTL_ADD, rev.data.fd, &rev) == -1) {
		switch (errno) {
			case EEXIST:	// attempt to add a handle twice
				break;
			default:
				throw (Exception("epoll_ctl"));
		}
	}
	_handles.emplace(handle);
}

void
Poller::modify(SharedHandle const& handle, EventTypes events, Modes modes) {
	Event::Raw			rev;
	Handle::Raw const	fd = handle->raw();

	rev.events = _get_bitmask(events) | _get_bitmask(modes);
	rev.data.fd = fd;
	if (epoll_ctl(raw(), EPOLL_CTL_MOD, fd, &rev) == -1) {
		switch (errno) {
			case ENOENT:	// attempt to modify an unregistered handle
				return;
			default:
				throw (Exception("epoll_ctl"));
		}
	}
}

void
Poller::remove(SharedHandle const& handle) {
	Event::Raw			rev; // man epoll_ctl -> BUGS
	Handle::Raw const	fd = handle->raw();

	if (epoll_ctl(raw(), EPOLL_CTL_DEL, fd, &rev) == -1)
		throw (Exception("epoll_ctl"));
	
	auto	it = std::find(_handles.begin(), _handles.end(), handle);

	_handles.erase(it);
}

size_t
Poller::size() const noexcept {
	return (_handles.size());
}
