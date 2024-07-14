#ifndef HTTP_CHUNK_HPP
# define HTTP_CHUNK_HPP

# include "Buffer.hpp"

# include <optional>
# include <sstream>
# include <stdexcept>

namespace http {
	constexpr size_t	chunk_margin = 8;
	// 4 for two CRLF + 4 for hex length indicator
	// As long as webserv::buffer_size does not exceed 65536 (0xFFFF), this
	// value won't need to be adjusted.

	using ChunkBuffer = network::Buffer<webserv::buffer_size - chunk_margin>;

	void	enchunk(ChunkBuffer const&, webserv::Buffer&);
	void	enchunk(std::ostream&, webserv::Buffer&);

	class Dechunker {
	public:
		enum class Status {
			pending, done,
		}; // enum class Status

		class ChunkException;

		Dechunker();

		Status	dechunk(webserv::Buffer&);

	private:
		using ChunkSize = std::optional<size_t>;

		bool	_get_size();

		ChunkSize			_size;
		std::stringstream	_buf;
	}; // class Dechunker

	class Dechunker::ChunkException: public std::exception {
	public:
		ChunkException(char const*);

		char const*	what() const noexcept;

	private:
		char const*	_msg;
	}; // class Dechunker::ChunkException

}; // namespace http

#endif // HTTP_CHUNK_HPP