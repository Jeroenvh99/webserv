#ifndef HTTP_CHUNK_HPP
# define HTTP_CHUNK_HPP

# include "Buffer.hpp"

# include <optional>
# include <sstream>

namespace http {
	webserv::Buffer	enchunk(webserv::ChunkBuffer const&);

	class Dechunker {
	public:
		using Chunk = std::optional<std::string>;
		class ChunkException;

		Dechunker();

		Chunk	dechunk(webserv::Buffer const&);

	private:
		using ChunkSize = std::optional<size_t>;

		bool	_get_size();

		ChunkSize			_size;
		std::stringstream	_buffer;
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
