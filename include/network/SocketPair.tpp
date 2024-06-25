#ifndef NETWORK_SOCKET_PAIR_TPP
# define NETWORK_SOCKET_PAIR_TPP

# ifndef NETWORK_SOCKET_HPP
#  error "include in network/Socket.hpp"
# endif // NETWORK_SOCKET_HPP

# include "network/utils.tpp"

namespace network {
	template<Domain DOMAIN, Type TYPE>
	SocketPair<DOMAIN, TYPE>::SocketPair(OptionList opts, char const* prot) {
		typename SocketType::Raw	fds[2];

		if (::socketpair(
			static_cast<int>(DOMAIN),
			static_cast<int>(TYPE) | _get_bitmask(opts),
			SocketType::_get_protocol(prot),
			fds) == -1)
			throw (SocketException("pair"));
		this->first = fds[0];
		this->second = fds[1];
	}
}; // namespace network

#endif // NETWORK_SOCKET_PAIR_HPP
