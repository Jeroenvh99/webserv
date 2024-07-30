#ifndef HTTP_CHUNK_HPP
# define HTTP_CHUNK_HPP

# include "Buffer.hpp"

# include <optional>
# include <sstream>
# include <stdexcept>

namespace http {
	webserv::Buffer&	enchunk(webserv::Buffer&, webserv::ChunkBuffer const&);

	class Dechunker {
	public:
		using Result = std::optional<size_t>;
		class Exception;

		Dechunker();

		std::ostream&	buffer() noexcept;

		Result	dechunk(webserv::Buffer&);
		Result	dechunk(webserv::Buffer&, std::istream&);
		Result	dechunk_single(webserv::Buffer&, std::istream&);

	private:
		void	get_size(std::istream&);
		size_t	get_end(std::istream&);

		std::optional<size_t>	_chunk_size;
		size_t					_bytes_dechunked;
		std::stringstream		_buf;
	}; // class Dechunker

	class Dechunker::Exception: public std::exception {
	public:
		Exception(char const*);

		char const*	what() const noexcept;
	private:
		char const*	_msg;
	}; // class Dechunker::ChunkException

}; // namespace http

#endif // HTTP_CHUNK_HPP