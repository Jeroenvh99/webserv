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
	constexpr size_t
	Buffer<SIZE>::capacity() {
		return (SIZE);
	}

	template<size_t SIZE>
	bool
	Buffer<SIZE>::push_back(char c) noexcept {
		if (len() >= capacity())
			return (false);
		this->data()[_len++] = c;
		return (true);
	}

	template<size_t SIZE>
	size_t
	Buffer<SIZE>::push_back(std::string const& str) noexcept {
		if (str.size() > capacity() - len())
			return (0);
		for (auto const& c: str)
			this->data()[_len++] = c;
		return (str.size());
	}

	template<size_t SIZE>
	template<size_t TSIZE>
	size_t
	Buffer<SIZE>::push_back(Buffer<TSIZE> const& that) noexcept {
		if (that.len() > capacity() - len())
			return (0);
		for (auto const& c: that)
			this->data()[_len++] = c;
		return (that.len());
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
	size_t
	Buffer<SIZE>::get(std::istream& is) {
		is.read(this->data(), SIZE); // signedness mismatch!
		_len = is.gcount();
		return (_len);
	}

	template<size_t SIZE>
	size_t
	Buffer<SIZE>::put(std::ostream& os) const {
		os.write(this->data(), _len); // signedness mismatch!
		return (os.good() ? _len : 0);
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
