#include "network/network.hpp"
#include "network/Handle.hpp"
#include "network/Socket.hpp"
#include "network/Poller.hpp"

#include <sstream>
extern "C" {
#include <string.h>
}

extern int 	errno;

using network::Handle;
using network::Poller;
using network::SocketException;

network::Exception::Exception(std::string const& msg):
	_msg(msg + ": " + strerror(errno)) {}

char const*
network::Exception::what() const noexcept {
	return (_msg.data());
}

Handle::Exception::Exception(char const* msg):
	network::Exception(msg) {}

Poller::Exception::Exception(char const* msg):
	network::Exception(msg) {}

SocketException::SocketException(char const* msg):
	network::Exception(msg) {}