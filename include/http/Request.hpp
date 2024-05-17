#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "http.hpp"

# include <iostream>
# include <stdexcept>
# include <string>
# include <sstream>
# include <unordered_map>
# include <utility>

namespace http {
	using Header = std::pair<std::string, std::string>;

	class Request {
	public:
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

	private:
		struct cmp {
			bool	operator()(std::string const&, std::string const&) const noexcept;
		}; // struct Request::cmp
		using Headers = std::unordered_map<
			std::string,
			std::string,
			std::hash<std::string>,
			cmp
		>;
		
		void	_header_add(Header&&);

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
		
		static std::iostream&	getline(std::iostream&, std::string&);

		void	clear() noexcept;
		void	parse(std::iostream&, Request&);

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
	bool			strcmp_nocase(std::string const&, std::string const&) noexcept;
	bool			is_ws(char c);
	std::string		trim_ws(std::string const&) noexcept;
	std::string&	trim_ws(std::string&) noexcept;
	std::string&	ltrim_ws(std::string&) noexcept;
	std::string&	rtrim_ws(std::string&) noexcept;
}; // namespace http

#endif // HTTP_REQUEST_HPP
