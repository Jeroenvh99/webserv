#ifndef NETWORK_ADDRESS_IPV4_HPP
# define NETWORK_ADDRESS_IPV4_HPP

# include "network.hpp"

extern "C" {
# include <arpa/inet.h>
}

namespace network {
	template<>
	class Address<Domain::ipv4> {
	public:
		Address() = default;
		Address(in_port_t, uint32_t);

		operator std::string() const noexcept;

		uint32_t	address() const noexcept;
		in_port_t	port() const noexcept;

		sockaddr const*		raw() const noexcept;
		static socklen_t	size();

	private:
		template<Domain DOMAIN, Type TYPE>
		friend class Socket;
		friend class Acceptor<Domain::ipv4>;

		Address(int);

		sockaddr*	raw() noexcept;
		
		sockaddr_in	_addr;
	}; // specialized class template SocketAddress<Domain::ipv4>
}; // namespace network

# include "network/Address_IPv4.tpp"

#endif // NETWORK_ADDRESS_IPV4_HPP
