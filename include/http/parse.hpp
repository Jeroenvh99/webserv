#ifndef HTTP_PARSE_HPP
# define HTTP_PARSE_HPP

# include "http/Message.hpp"
# include "http/Request.hpp"

# include <iostream>
# include <optional>

namespace http {
	std::optional<Header>	parse_header_cgi(std::iostream&);

	class Parser {
	public:
		enum class State;
		
		class Exception;
		class IncompleteLineException; // DB: replace with std::optional
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

		State	_state;
		Header	_tmp_hdr;
	}; // class Parser

	enum class Parser::State {
		start,
		header,
		done,
	}; // enum class Parser::State

	class Parser::Exception: public std::logic_error {
	public:
		Exception(char const* = "");
	}; // class Parser::Exception

	class Parser::IncompleteLineException:
			public Parser::Exception {
	}; // class Parser::IncompleteLineException

	class Parser::StartLineException:
			public Parser::Exception {
	public:
		StartLineException(char const*);
	}; // class Parser::StartLineException

	class Parser::MethodException:
			public Parser::StartLineException {
	public:
		MethodException(char const*);
	}; // class Parser::MethodException

	class Parser::VersionException:
			public Parser::StartLineException {
	public:
		VersionException(char const*);
	}; // class Parser::VersionException

	class Parser::HeaderException:
			public Parser::Exception {
	public:
		HeaderException(char const*);
	}; // class Parser::HeaderException
}; // namespace http

#endif // HTTP_PARSER_HPP