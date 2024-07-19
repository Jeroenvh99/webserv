#ifndef BUFFER_HPP
# define BUFFER_HPP

# include "network/Buffer.hpp"

namespace webserv {
	constexpr size_t	buffer_size = 4092;
	constexpr size_t	chunk_margin = 7; // 4 for two CRLF + 3 for hex length indicator (maximum value = 3F9)

	using Buffer = network::Buffer<buffer_size>;
	using ChunkBuffer = network::Buffer<buffer_size - chunk_margin>;
}; // namespace webserv

#endif // BUFFER_HPP