#ifndef NETWORK_BUFFER_TPP
# define NETWORK_BUFFER_TPP

# ifndef NETWORK_BUFFER_HPP
#  error "include __FILE__ in network/Buffer.hpp"
# endif // NETWORK_BUFFER_HPP

# include <sstream>

namespace network {
	template<size_t SIZE>
	Buffer<SIZE>::Buffer():
		super(), _len(0) {}
	
	template<size_t SIZE>
	Buffer<SIZE>::operator std::string() const {
		return (std::string(this->begin(), this->end()));
	}

	template<size_t SIZE>
	void
	Buffer<SIZE>::empty() noexcept {
		_len = 0;
	}

	template<size_t SIZE>
	size_t
	Buffer<SIZE>::len() const noexcept {
		return (_len);
	}
	
	template<size_t SIZE>
	typename Buffer<SIZE>::iterator
	Buffer<SIZE>::begin() noexcept {
		return (super::begin());
	}
	
	template<size_t SIZE>
	typename Buffer<SIZE>::const_iterator
	Buffer<SIZE>::begin() const noexcept {
		return (super::begin());
	}
	
	template<size_t SIZE>
	typename Buffer<SIZE>::const_iterator
	Buffer<SIZE>::cbegin() const noexcept {
		return (super::begin());
	}

	template<size_t SIZE>
	typename Buffer<SIZE>::iterator
	Buffer<SIZE>::end() noexcept {
		return (begin() + _len);
	}
	template<size_t SIZE>
	typename Buffer<SIZE>::const_iterator
	Buffer<SIZE>::end() const noexcept {
		return (begin() + _len);
	}
	template<size_t SIZE>
	typename Buffer<SIZE>::const_iterator
	Buffer<SIZE>::cend() const noexcept {
		return (cbegin() + _len);
	}

	template<size_t SIZE>
	std::istream&
	operator>>(std::istream& is, Buffer<SIZE>& that) {
		while (!is.eof() && that._len < SIZE)
			that[that._len++] = is.get();
		return (is);
	}

	template<size_t SIZE>
	std::ostream&
	operator<<(std::ostream& os, Buffer<SIZE> const& that) {
		for (auto const& byte: that)
			os << byte;
		return (os);
	}
}; // namespace network

#endif // NETWORK_BUFFER_TPP
