#ifndef NETWORK_BUFFER_HPP
# define NETWORK_BUFFER_HPP

# include "network.hpp"

# include <array>
# include <istream>
# include <ostream>
# include <string>

namespace network {
	template<size_t SIZE>
	class Buffer: public std::array<char, SIZE> {
	public:
		using super = std::array<char, SIZE>;
		using iterator = typename super::iterator;
		using const_iterator = typename super::const_iterator;

		Buffer();

		operator std::string() const;

		void					empty() noexcept;
		size_t					len() const noexcept;
		static constexpr size_t	capacity();

		iterator		begin() noexcept;
		const_iterator	begin() const noexcept;
		const_iterator	cbegin() const noexcept;
		iterator		end() noexcept;
		const_iterator	end() const noexcept;
		const_iterator	cend() const noexcept;

		size_t	get(std::istream&);
		size_t	put(std::ostream&) const;

		bool	push_back(char) noexcept;
		size_t	push_back(std::string const&) noexcept;
		template<size_t TSIZE>
		size_t	push_back(Buffer<TSIZE> const&) noexcept;

	private:
		template<Domain DOMAIN, Type TYPE>
		friend class Socket;
		template<size_t S>
		friend std::istream&	operator>>(std::istream&, Buffer<S>&);

		size_t	_len;
	}; // class template Buffer

	template<size_t SIZE>
	std::istream&	operator>>(std::istream&, Buffer<SIZE>&);
	template<size_t SIZE>
	std::ostream&	operator<<(std::ostream&, Buffer<SIZE> const&);

	// add dynamic buffer?

}; // namespace network

# include "network/Buffer.tpp"

#endif // NETWORK_BUFFER_HPP
