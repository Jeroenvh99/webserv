#ifndef NETWORK_ACCEPTOR_TPP
# define NETWORK_ACCEPTOR_TPP

# ifndef NETWORK_ACCEPTOR_HPP
#  error "include in network/Acceptor.hpp"
# endif // NETWORK_ACCEPTOR_HPP

namespace network {
	template<Domain DOMAIN>
	Acceptor<DOMAIN>::Acceptor(OptionList opts, char const *prot):
		StreamSocket<DOMAIN>(opts, prot) {}

	template<Domain DOMAIN>
	Acceptor<DOMAIN>::Acceptor(Address const& addr, OptionList opts, char const *prot):
		StreamSocket<DOMAIN>(addr, opts, prot) {}

	template<Domain DOMAIN>
	Acceptor<DOMAIN>::Acceptor(int backlog, Address const& addr, OptionList opts, char const *prot):
		StreamSocket<DOMAIN>(addr, opts, prot) {
		listen(backlog);
	}

	template<Domain DOMAIN>
	void
	Acceptor<DOMAIN>::listen(int backlog) const {
		if (::listen(this->raw(), backlog) == -1) {
			throw (SocketException("listen"));
		}
	}

	template<Domain DOMAIN>
	StreamSocket<DOMAIN>
	Acceptor<DOMAIN>::accept() const {
		Handle::Raw const	raw_handle = ::accept(this->raw(), nullptr, nullptr);
		
		if (raw_handle == StreamSocket<DOMAIN>::_invalid_handle)
			throw (SocketException("accept"));
		return (StreamSocket<DOMAIN>(raw_handle));
	}

	template<Domain DOMAIN>
	StreamSocket<DOMAIN>
	Acceptor<DOMAIN>::accept(typename StreamSocket<DOMAIN>::Address& addr) const {
		socklen_t			size = addr.size();
		Handle::Raw const	raw_handle = ::accept(this->raw(), addr.raw(), &size);

		if (raw_handle == StreamSocket<DOMAIN>::_invalid_handle)
			throw (SocketException("accept"));
		if (size != addr.size())
			throw (network::Exception("accept"));
		return (StreamSocket<DOMAIN>(raw_handle));
	}
}; // namespace network

#endif // NETWORK_ACCEPTOR_TPP
