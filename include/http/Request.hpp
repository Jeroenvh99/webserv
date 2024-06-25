#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "http.hpp"
# include "http/Message.hpp"
# include "http/Body.hpp"
# include "URI.hpp"

# include <iostream>
# include <stdexcept>
# include <string>
# include <sstream>
# include <unordered_map>
# include <utility>

namespace http {
	class Request: public Message {
	public:
		class Parser;

		Request(Method = Method::GET,
			Version = Version(0, 0),
			std::string const& = "");
		Request(Method, Version, std::string&&);

		operator std::string() const noexcept;

		Method				method() const noexcept;
		Version				version() const noexcept;
		URI const&			uri() const noexcept;
		std::string const&	header(std::string const&) const;
		HeaderMap const&	headers() const noexcept;
		bool				has_header(std::string const&) const noexcept;
		Body				has_body() const; // this is not particularly speedy
		size_t				header_count() const noexcept;

		void	clear() noexcept;

	private:
		void	_header_append(Header&&);

		Method				_method;
		Version				_version;
		URI					_uri;
		HeaderMap 			_headers;
	}; // class Request

	class Request::Parser {
	public:
		enum class State;
		
		class Exception;
		class IncompleteLineException;
		class HeaderException;
		class StartLineException;
		class MethodException;
		class VersionException;

		Parser();
		
		static std::iostream&	getline(std::iostream&, std::string&);

		void	clear() noexcept;
		State	parse(std::iostream&, Request&);

		State	state() const noexcept;

	private:
		Request	_parse_start(std::iostream&);
		void	_parse_headers(std::iostream&, Request&);
		void	_header_add(Request&);
		void	_parse_body(std::iostream&, Request&);
		void	_parse_chunks(std::iostream&, std::string&);

		State	_state;
		Header	_tmp_hdr; 		// union
		size_t	_body_length;	// "
	}; // class Request::Parser

	enum class Request::Parser::State {
		start,			// parsing the first line
		header,			// parsing headers
		body_by_length,	// parsing body, checking number of bytes
		body_chunked,	// parsing body, processing chunks
		body_until_eof, // parsing body until connection is closed
		done,			// parsing has finished
	}; // enum class Request::Parser::State

	class Request::Parser::Exception: public std::logic_error {
	public:
		Exception(char const* = "");
	}; // class Request::Parser::Exception

	class Request::Parser::IncompleteLineException:
			public Request::Parser::Exception {
	}; // class Request::Parser::IncompleteLineException

	class Request::Parser::StartLineException:
			public Request::Parser::Exception {
	public:
		StartLineException(char const*);
	}; // class Request::Parser::StartLineException

	class Request::Parser::MethodException:
			public Request::Parser::StartLineException {
	public:
		MethodException(char const*);
	}; // class Request::Parser::MethodException

	class Request::Parser::VersionException:
			public Request::Parser::StartLineException {
	public:
		VersionException(char const*);
	}; // class Request::Parser::VersionException

	class Request::Parser::HeaderException:
			public Request::Parser::Exception {
	public:
		HeaderException(char const*);
	}; // class Request::Parser::HeaderException
}; // namespace http

#endif // HTTP_REQUEST_HPP
