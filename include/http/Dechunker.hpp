#ifndef HTTP_DECHUNKER_HPP
# define HTTP_DECHUNKER_HPP

# include <optional>
# include <sstream>
# include <stdexcept>

namespace http {
	class Dechunker {
	public:
		using Chunk = std::optional<std::string>;
		class ChunkException;

		Dechunker();

		Chunk	dechunk(webserv::Buffer const&);

	private:
		using ChunkSize = std::optional<size_t>;

		void	_get_size();

		ChunkSize			_size;
		std::stringstream	_buffer;
	}; // class Dechunker

	class ChunkException: public std::exception {
	public:
		ChunkException(char const*);

		char const*	what() const noexcept;

	private:
		char const*	_msg;
	}

}; // namespace http

#endif // HTTP_DECHUNKER_HPP