#ifndef NETWORK_SOCKET_HPP
# define NETWORK_SOCKET_HPP

# include <initializer_list>
# include <stdexcept>
# include <utility>

# include "network.hpp"
# include "network/Handle.hpp"

namespace network {
	enum class SocketOption: int;
	class SocketException;
	
	template<Domain DOMAIN, Type TYPE>
	class Socket: public Handle {
	public:
		using OptionList = std::initializer_list<SocketOption>;
		using Address = network::Address<DOMAIN>;

		Socket(OptionList = {}, char const* = "");
		Socket(Address const&, OptionList = {}, char const* = "");

		void	bind(Address const&);

		static std::pair<Socket, Socket>	pair(OptionList, char const*);

		template<size_t BSIZE>
		size_t	write(Buffer<BSIZE> const&, int = 0) const;
		template<size_t BSIZE>
		size_t	read(Buffer<BSIZE>&, int = 0) const;

		Address	address() const noexcept;
		bool	listens() const noexcept;  // would use forbidden function
		int		protocol() const noexcept; // would use forbidden function

	protected:
		template<Domain, Type>
		friend class SocketPair;
		using typename Handle::Raw;

		Socket(Raw);

	private:
		static Raw	_new_handle(OptionList, char const*) noexcept;
		static int	_get_protocol(char const*) noexcept;
	}; // class template Socket

	template<Domain DOMAIN, Type TYPE>
	class SocketPair: public std::pair<Socket<DOMAIN, TYPE>, Socket<DOMAIN, TYPE>> {
	public:
		using SocketType = Socket<DOMAIN, TYPE>;
		using OptionList = typename SocketType::OptionList;

		SocketPair(OptionList = {}, char const* = "");
	}; // class template SocketPair

	enum class SocketOption: int {
		nonblock = SOCK_NONBLOCK,
		cloexec = SOCK_CLOEXEC,
	}; // enum class SocketOption

	class SocketException: public network::Exception {
	public:
		SocketException(char const*);
	}; // class Socket::Exception
}; // namespace network

# include "network/Socket.tpp"
# include "network/SocketPair.tpp"

#endif // NETWORK_SOCKET_HPP
