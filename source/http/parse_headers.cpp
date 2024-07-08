#include "http.hpp"
#include "http/parse.hpp"

#include <sstream>

using http::Request;
using http::Method;
using http::Parser;
using http::Version;
using http::Header;

enum class HeaderLineType {
	first,
	continuation,
	end,
}; // enum class HeaderLineType

static HeaderLineType	_get_header_line(std::iostream&, std::string&);
static void				_amend_header(Header::Value&, std::string const&);

void
Parser::_parse_headers(std::iostream& ios, Request& req) {
	std::string	line;

	while (true) {
		switch (_get_header_line(ios, line)) {
		case HeaderLineType::first:
			try {
				_current_header = req.headers().insert(line);
			} catch (std::invalid_argument&) {
				_current_header = std::nullopt;
				throw (Parser::HeaderException("bad header format"));
			}
			break;
		case HeaderLineType::continuation:
			_amend_header(_current_header.value()->second, line);
			break;
		case HeaderLineType::end:
			_current_header = std::nullopt;
			_state = State::done;
			return;
		}
	}
}

static HeaderLineType
_get_header_line(std::iostream& ios, std::string& line) {
	Parser::getline(ios, line);
	if (line.size() == 0)		// bare CRLF
		return (HeaderLineType::end);
	if (http::is_ws(line[0]))	// leading whitespace
		return (HeaderLineType::continuation);
	return (HeaderLineType::first);
}

static void
_amend_header(Header::Value& values, std::string const& str) {
	std::istringstream	iss(str);
	std::string			sval;

	while (std::getline(iss, sval, ','))
		values.insert(std::move(sval));
}