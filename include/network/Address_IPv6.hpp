#ifndef NETWORK_ADDRESS_IPV6_HPP
# define NETWORK_ADDRESS_IPV6_HPP

# include "network.hpp"
# include "network/AddressBase.hpp"

extern "C" {
# include <arpa/inet.h>
}

namespace network {
	template<>
	class Address<Domain::ipv6>: public AddressBase {
	public:
		Address() = default;
		Address(in_port_t, uint32_t = INADDR_ANY);
		Address(SocketAddressAny const&);

		operator std::string() const noexcept;

		uint32_t	address() const noexcept;
		in_port_t	port() const noexcept;

		sockaddr const*	pointer() const noexcept;
		sockaddr*		pointer() noexcept;
		socklen_t		size() const noexcept;

	private:
		sockaddr_in6	_addr;
	}; // specialized class template SocketAddress<Domain::ipv4>
}; // namespace network

# include "network/Address_IPv6.tpp"

#endif // NETWORK_ADDRESS_IPV6_HPP
