#include "network/Poller.hpp"

extern "C" {
# include <fcntl.h>
}

using network::Poller;

Poller::Poller():
	Handle(::epoll_create(1)) {
	if (::fcntl(raw(), F_SETFD, O_CLOEXEC) == -1) // If only I were allowed to use epoll_create1()...
		throw (Exception("fcntl"));
}

Poller::Poller(Poller&& that):
	Handle(static_cast<Handle&&>(that)) {}

Poller&
Poller::operator=(Poller&& that) {
	Handle::operator=(static_cast<Handle&&>(that));
	return (*this);
}
