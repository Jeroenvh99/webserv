#ifndef NETWORK_SOCKET_PAIR_TPP
# define NETWORK_SOCKET_PAIR_TPP

# ifndef NETWORK_SOCKET_HPP
#  error "include in network/Socket.hpp"
# endif // NETWORK_SOCKET_HPP

# include "network/utils.tpp"

namespace network {
	template<typename SOCKET>
	SocketPair<SOCKET>::SocketPair(OptionList opts, char const* prot) {
		typename SocketClass::Raw	fds[2];

		if (::socketpair(
			static_cast<int>(domain),
			static_cast<int>(type) | _get_bitmask(opts),
			SocketClass::_get_protocol(prot),
			fds) == -1)
			throw (SocketException("pair"));
		this->first = fds[0];
		this->second = fds[1];
	}
}; // namespace network

#endif // NETWORK_SOCKET_PAIR_HPP
