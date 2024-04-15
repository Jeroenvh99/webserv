#ifndef NETWORK_ADDRESS_BASE_HPP
# define NETWORK_ADDRESS_BASE_HPP

# include "network.hpp"

extern "C" {
# include <sys/socket.h>
}

namespace network {
	class AddressBase {
	public:
		virtual ~AddressBase() = default;

		virtual sockaddr const*		raw() const noexcept = 0;
		virtual socklen_t	size() const noexcept = 0;
	}; // class Address
}; // namespace network

#endif // NETWORK_ADDRESS_BASE_HPP
