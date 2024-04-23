#ifndef NETWORK_ACCEPTOR_HPP
# define NETWORK_ACCEPTOR_HPP

# include "network.hpp"
# include "network/StreamSocket.hpp"

namespace network {
	template<Domain DOMAIN>
	class Acceptor: public StreamSocket<DOMAIN> {
	public:
		using super = StreamSocket<DOMAIN>;
		using typename super::Address;
		using typename super::OptionList;

		Acceptor(OptionList = {}, char const* = nullptr);
		Acceptor(Address const&, OptionList = {}, char const* = nullptr);
		Acceptor(int, Address const&, OptionList = {}, char const* = nullptr);

		void					listen(int) const;
		StreamSocket<DOMAIN>	accept() const;
		StreamSocket<DOMAIN>	accept(typename StreamSocket<DOMAIN>::Address&) const;
 
		template<size_t BSIZE>
		void			read(Buffer<BSIZE> const&, int) = delete;
		template<size_t BSIZE>
		size_t			write(Buffer<BSIZE>&, int) = delete;
	}; // class template Acceptor<Domain>
}; // namespace network

# include "network/Acceptor.tpp"

#endif // NETWORK_ACCEPTOR_HPP
