#ifndef HTTP_PARSE_HPP
# define HTTP_PARSE_HPP

# include "http.hpp"
# include "http/Header.hpp"
# include "http/Request.hpp"
# include "http/Response.hpp"

# include "utils.hpp"

# include <iostream>
# include <optional>

namespace http::parse {
	class HeaderParser {
	public:
		enum class Status {busy, done};

		void	clear() noexcept;

		Status	parse(std::iostream&, Headers&);
		Status	parse_cgi(std::iostream&, Headers&);

	private:
		std::string	_buf;
	}; // class HeaderParser

	class RequestParser {
	public:
		enum class State;

		RequestParser();

		State	state() const noexcept;

		void	clear() noexcept;
		State	parse(std::iostream&, Request&);

	private:
		Request	_parse_request_line(std::iostream&);

		State			_state;
		HeaderParser	_header_parser;
	}; // class Parser

	enum class RequestParser::State {
		start,
		header,
		done,
	}; // enum class RequestParser::State

	class Exception: public std::exception {
	public:
		Exception(char const* = "");

		char const*	what() const noexcept;
	private:
		char const*	_msg;
	}; // class Exception

	class MethodException: public Exception {
	public:
		MethodException(char const*);
	}; // class MethodException

	class VersionException: public Exception {
	public:
		VersionException(char const*);
	}; // class VersionException

	class HeaderException: public Exception {
	public:
		HeaderException(char const*);
	}; // class HeaderException
}; // namespace http::parse

#endif // HTTP_PARSE_HPP