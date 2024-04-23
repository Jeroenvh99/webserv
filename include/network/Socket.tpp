#ifndef NETWORK_SOCKET_TPP
# define NETWORK_SOCKET_TPP

# ifndef NETWORK_SOCKET_HPP
#  error "include in network/Socket.hpp"
# endif // NETWORK_SOCKET_HPP

# include "network/utils.tpp"

extern "C" {
#include <netdb.h>
#include <unistd.h>
}

namespace network {
	// Protected basic operations

	template<Domain DOMAIN, Type TYPE>
	Socket<DOMAIN, TYPE>::Socket(Raw raw):
		Handle(raw) {}

	template<Domain DOMAIN, Type TYPE>
	Socket<DOMAIN, TYPE>::Socket(OptionList opts, char const* prot):
		Handle(_new_handle(opts, prot)) {}
	
	template<Domain DOMAIN, Type TYPE>
	Socket<DOMAIN, TYPE>::Socket(Address const& addr, OptionList opts, char const* prot):
		Handle(_new_handle(opts, prot)) {
		bind(addr);
	}

	// Public methods
	
	template<Domain DOMAIN, Type TYPE>
	void
	Socket<DOMAIN, TYPE>::bind(Address const& addr) {
		if (::bind(raw(), addr.raw(), addr.size()) == -1) {
			throw (SocketException("bind"));
		}
	}

	template<Domain DOMAIN, Type TYPE>
	typename Socket<DOMAIN, TYPE>::Address
	Socket<DOMAIN, TYPE>::address() const noexcept {
		return Address(this->raw());
	}

	template<Domain DOMAIN, Type TYPE>
	template<size_t BSIZE>
	size_t
	Socket<DOMAIN, TYPE>::write(Buffer<BSIZE> const& buf, int flags) const {
		ssize_t const	len = send(raw(), buf.data(), buf._len, flags);

		if (good() && len == -1) {
			throw (SocketException("send"));
		}
		return (len);
	}

	template<Domain DOMAIN, Type TYPE>
	template<size_t BSIZE>
	void
	Socket<DOMAIN, TYPE>::read(Buffer<BSIZE>& buf, int flags) const {
		ssize_t const	len = recv(raw(), buf.data(), BSIZE, flags);

		if (good() && len == -1) {
			throw (SocketException("recv"));
		}
		buf._len = len;
	}

	// Private and protected methods

	template<Domain DOMAIN, Type TYPE>
	typename Socket<DOMAIN, TYPE>::Raw
	Socket<DOMAIN, TYPE>::_new_handle(OptionList opts, char const* prot) noexcept {
		return (::socket(
			static_cast<int>(DOMAIN),
			static_cast<int>(TYPE) | _get_bitmask(opts),
			_get_protocol(prot))
		);
	}

	template<Domain DOMAIN, Type TYPE>
	int
	Socket<DOMAIN, TYPE>::_get_protocol(char const* prot) noexcept {
		if (!prot || prot[0] == '\0')
			return (0);
		protoent const*	const	entry = getprotobyname(prot);

		//if (entry == nullptr)
		//	throw (SocketException("getprotobyname"));
		return (entry->p_proto);
	}
}; // namespace network

#endif // NETWORK_SOCKET_TPP
