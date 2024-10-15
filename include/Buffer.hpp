#ifndef BUFFER_HPP
# define BUFFER_HPP

# include "network/Buffer.hpp"

namespace webserv {
	constexpr size_t	buffer_size = 1024;

	using Buffer = network::Buffer<buffer_size>;
}; // namespace webserv

#endif // BUFFER_HPP