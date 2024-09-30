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

		class Exception;

		Dechunker();

		void	clear() noexcept;
		Status	dechunk(webserv::Buffer&);

	private:
		Status	dechunk_core(webserv::Buffer&);
		Status	dechunk_one(webserv::Buffer&);
		void	extract_size();
		void	extract_terminator();

		std::stringstream		_buf;
		std::optional<size_t>	_remaining;
		bool					_found_null;
	}; // class Dechunker

	class Dechunker::Exception: public std::exception {
	public:
		Exception(char const*);

		char const*	what() const noexcept;
	private:
		char const*	_msg;
	}; // class Dechunker::Exception

}; // namespace http

#endif // HTTP_CHUNK_HPP
