#include "http.hpp"
#include "http/parse.hpp"

#include <sstream>

using http::Header;
using http::Method;
using http::Request;
using http::Version;
using http::parse::Parser;

enum class HeaderLineType {
	first,
	continuation,
	end,
}; // enum class HeaderLineType

static HeaderLineType	_get_header_line(std::iostream&, std::string&);

void
Parser::_parse_headers(std::iostream& ios, Request& req) {
	std::string	line;

	while (true) {
		switch (_get_header_line(ios, line)) {
		case HeaderLineType::first:
			try {
				if (_header_buffer.length() > 0)
					req.headers().update(_header_buffer);
				_header_buffer = line;
			} catch (std::invalid_argument&) {
				throw (HeaderException("bad header format"));
			}
			break;
		case HeaderLineType::continuation:
			_header_buffer += line;
			break;
		case HeaderLineType::end:
			_header_buffer.clear();
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