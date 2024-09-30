#ifndef HTTP_CHUNK_HPP
# define HTTP_CHUNK_HPP

# include "Buffer.hpp"

# include <optional>
# include <sstream>

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

		class Exception;

		Dechunker();

		Status	dechunk(webserv::Buffer&);

	private:
		void	get_size(std::istream&);
		size_t	get_end(std::istream&);

		bool	_get_size();

		std::stringstream	_buf;
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
