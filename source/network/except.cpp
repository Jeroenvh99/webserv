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

static std::string	_concat(char const*, char const*);

network::Exception::Exception(char const* hdr, char const* msg):
	std::runtime_error(_concat(hdr, msg)) {}

Handle::Exception::Exception(char const* msg):
	network::Exception("handle", msg) {}

Poller::Exception::Exception(char const* msg):
	network::Exception("poll", msg) {}

SocketException::SocketException(char const* msg):
	network::Exception("socket", msg) {}

static std::string
_concat(char const* hdr, char const* msg) {
	if (hdr[0] == '\0')
		return (std::string(msg));
	std::ostringstream	oss(hdr);

	oss << ": " << msg << ": " << strerror(errno);
	return (oss.str());
}
