#ifndef NETWORK_STREAM_SOCKET_HPP
# define NETWORK_STREAM_SOCKET_HPP

# include "network.hpp"
# include "network/Socket.hpp"

namespace network {
	template<Domain DOMAIN>
	class StreamSocket: public Socket<DOMAIN, Type::stream> {
	public:
		using super = Socket<DOMAIN, Type::stream>;
		using typename super::Address;
		using typename super::OptionList;

		StreamSocket(OptionList = {}, char const* = nullptr);
		StreamSocket(Address const&, OptionList = {}, char const* = nullptr);

	protected:
		friend class Acceptor<DOMAIN>;
		using typename super::Raw;

		StreamSocket(Raw);
	}; // class template StreamSocket<Domain>
}; // namespace network

# include "StreamSocket.tpp"

#endif // NETWORK_STREAM_SOCKET_HPP
