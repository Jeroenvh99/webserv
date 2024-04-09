#include "Acceptor.hpp"

using network::Socket;

Acceptor::Acceptor(network::IPv4SocketAddress const& address):
	network::Acceptor(
		Socket::Domain::ipv4,
		{Socket::Option::cloexec}) {
	bind(address);
}
