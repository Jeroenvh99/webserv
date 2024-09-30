#include "network/Address_IPv4.hpp"

#include <iostream>
#include <sstream>

using network::Address;
using network::Domain;

// Basic operations

Address<Domain::ipv4>::Address(in_port_t port, uint32_t address) {
	_addr.sin_family = static_cast<int>(Domain::ipv4);
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = htonl(address);
}

Address<Domain::ipv4>::Address(int sd) {
	socklen_t	asize = size();

	if (::getsockname(sd, reinterpret_cast<sockaddr*>(&_addr), &asize) == -1
		|| asize > size()) {
		throw (network::Exception("getsockname")); // dedicated exception?
	}
}

// Conversion operators

Address<Domain::ipv4>::operator std::string() const noexcept {
	std::ostringstream	oss;
	uint32_t const		addr = address();
	uint8_t const*		octets = reinterpret_cast<uint8_t const*>(&addr);

	for (size_t i = 3; i > 0; --i)
		oss << std::to_string(octets[i]) << '.';
	oss << std::to_string(octets[0]) << ':' << std::to_string(port());
	return (oss.str());
}

// Public methods

in_port_t
Address<Domain::ipv4>::port() const noexcept {
	return (ntohs(_addr.sin_port));
}

uint32_t
Address<Domain::ipv4>::address() const noexcept {
	return (ntohl(_addr.sin_addr.s_addr));
}

sockaddr const*
Address<Domain::ipv4>::raw() const noexcept {
	return (reinterpret_cast<sockaddr const*>(&_addr));
}

socklen_t
Address<Domain::ipv4>::size() {
	return (sizeof(sockaddr_in));
}

sockaddr*
Address<Domain::ipv4>::raw() noexcept {
	return (reinterpret_cast<sockaddr*>(&_addr));
}
