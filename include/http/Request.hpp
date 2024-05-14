#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "http.hpp"
# include "Buffer.hpp"

# include <iostream>
# include <stdexcept>
# include <string>
# include <sstream>
# include <unordered_map>
# include <utility>

namespace http
{
	class Request {
	public:
		using Header = std::pair<std::string, std::string>;
		class Parser;

		Request(Method = Method::GET,
			Version = Version(0, 0),
			std::string const& = "");
		Request(Method, Version, std::string&&);

		operator std::string() const noexcept;

		Method				method() const noexcept;
		Version				version() const noexcept;
		std::string const&	uri() const noexcept;
		std::string const&	header(std::string const&) const;
		bool				has_header(std::string const&) const noexcept;
		std::string const&	body() const noexcept;
		std::string&		body() noexcept;

		void	clear() noexcept;
		void	header_add(Header const&);
		void	header_add(Header&&);
		void	header_add(std::string const&, std::string const&);
		void	header_add(std::string&&, std::string&&);

	private:
		struct cmp {
			bool	operator()(std::string const&, std::string const&) const;
		}; // struct Request::cmp
		using Headers = std::unordered_map<std::string, std::string, std::hash<std::string>, cmp>;

		Method		_method;
		Version		_version;
		std::string _uri;
		Headers 	_headers;
		std::string _body;
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
		~Parser();
		Parser(Parser const&) = delete;
		Parser(Parser&&) = delete;
		Parser&	operator=(Parser const&) = delete;
		Parser&	operator=(Parser&&) = delete;

		void	append(Buffer const&);
		void	clear() noexcept;
		// make some of these private
		void	parse(Buffer const&, Request&);
		Request	parse_start();
		void	parse_headers(Request&);
		void	parse_body(Request&);

		bool	needs_body(Request const&);

		State	state() const noexcept;

	private:
		void	_parse_header(Request&, std::string const&);

		State				_state;
		std::pair<std::string, std::string>	_tmp_hdr; 		// union
		size_t				_body_length;	// "
		size_t				_chunk_length;	// "
		std::stringstream	_buffer;
	}; // class Request::Parser

	enum class Request::Parser::State {
		start,			// parsing the first line
		header,			// parsing headers
		body_by_length,	// parsing body, checking number of bytes
		body_chunked,	// parsing body, processing chunks
		body_until_eof, // parsing body until connection is closed
		done,			// parsing has finished
	}; // enum class Request::Parser::BodyParserMode

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

	std::istream&	getline(std::istream&, std::string&);
	bool			strcmp_nocase(std::string const&, std::string const&);
	std::string		trim_ws(std::string const&);
	std::string&	trim_ws(std::string&);
	std::string&	ltrim_ws(std::string&);
	std::string&	rtrim_ws(std::string&);
}; // namespace http

#endif // HTTP_REQUEST_HPP
