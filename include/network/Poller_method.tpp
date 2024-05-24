#ifndef NETWORK_POLLER_METHOD_TPP
# define NETWORK_POLLER_METHOD_TPP

# ifndef NETWORK_POLLER_HPP
#  error "include in network/Poller.hpp"
# endif // NETWORK_POLLER_HPP

# include "network/utils.tpp"

# include <algorithm>

namespace network {
	template<typename T> // Requires a Handle-derived type.
	SharedHandle
	Poller::add(T&& handle, EventTypes events, Modes modes) {
	SharedHandle	box = std::make_shared<T>(handle);
	
	add_shared(box, events, modes);
	return (box);
	}
	
	template<size_t MAX_EVENTS>
	Poller::Events
	Poller::wait(unsigned int timeout) const {
		Events		evs;
		Event::Raw	revs[MAX_EVENTS];
		int	const	n = epoll_wait(raw(), revs, MAX_EVENTS, timeout);

		if (n == -1)
			throw (Exception("epoll_wait"));
		evs.reserve(n);
		for (int i = 0; i < n; ++i) {
			auto	is_match = [fd = revs[i].data.fd](SharedHandle const& shandle) {
				return (*shandle == fd);
			};
			auto	shandle = std::find_if(_handles.begin(), _handles.end(), is_match);

			if (shandle == _handles.end()) {
				throw (std::runtime_error("polled handle not found in list"));
			}
			evs.push_back(Event(*shandle, revs[i]));
		}
		return (evs);
	}
}; // namespace network

#endif // NETWORK_POLLER_METHOD_TPP
