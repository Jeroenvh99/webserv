#ifndef HTTP_PARSE_HPP
# define HTTP_PARSE_HPP

# include "http/Message.hpp"
# include "http/Request.hpp"

# include <iostream>
# include <optional>

namespace http::parse {
	class Parser {
	public:
		enum class State;

		Parser();
		
		static std::iostream&	getline(std::iostream&, std::string&);

		void	clear() noexcept;
		State	parse(std::iostream&, Request&);

		State	state() const noexcept;

	private:
		Request	_parse_start(std::iostream&);
		void	_parse_headers(std::iostream&, Request&);

		State		_state;
		std::string	_header_buffer;
	}; // class Parser

	enum class Parser::State {
		start,
		header,
		done,
	}; // enum class Parser::State

	class Exception: public std::exception {
	public:
		Exception(char const* = "");

		char const*	what() const noexcept;
	private:
		char const*	_msg;
	}; // class Exception

	class IncompleteLineException: public std::exception {};

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