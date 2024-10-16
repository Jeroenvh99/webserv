#ifndef BUFFER_HPP
# define BUFFER_HPP

# include "network/Buffer.hpp"

namespace webserv {
	constexpr size_t	buffer_size = 65536;
	constexpr size_t	chunk_margin = 8; // 4 for two CRLF + 4 for hex length indicator (supports a base buffer size of up to 65536)

	using Buffer = network::Buffer<buffer_size>;
	using ChunkBuffer = network::Buffer<buffer_size - chunk_margin>;
}; // namespace webserv

#endif // BUFFER_HPP