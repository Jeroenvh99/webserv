#ifndef NETWORK_STREAM_SOCKET_TPP
# define NETWORK_STREAM_SOCKET_TPP

# ifndef NETWORK_STREAM_SOCKET_HPP
#  error "include in network/StreamSocket.hpp"
# endif // NETWORK_STREAM_SOCKET_HPP

namespace network {
	template<Domain DOMAIN>
	StreamSocket<DOMAIN>::StreamSocket(Raw raw):
		Socket<DOMAIN, Type::stream>(raw) {}

	template<Domain DOMAIN>
	StreamSocket<DOMAIN>::StreamSocket(OptionList opts, char const* prot):
		Socket<DOMAIN, Type::stream>(opts, prot) {}

	template<Domain DOMAIN>
	StreamSocket<DOMAIN>::StreamSocket(Address const& addr, OptionList opts, char const* prot):
		Socket<DOMAIN, Type::stream>(addr, opts, prot) {}
}; // namespace network

#endif // NETWORK_STREAM_SOCKET_TPP
