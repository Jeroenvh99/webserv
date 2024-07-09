#include "http/parse.hpp"

#include <sstream>

using http::parse::HeaderParser;

enum class HeaderLineType {
	first,
	continuation,
	end,
}; // enum class HeaderLineType

static HeaderLineType	_getheaderline(std::iostream&, std::string&);

void
HeaderParser::clear() noexcept {
	_buf.clear();
}

HeaderParser::Status
HeaderParser::parse(std::iostream& ios, Headers& hdrs) {
	std::string	line;

	while (true) {
		switch (_getheaderline(ios, line)) {
		case HeaderLineType::first:
			if (_buf.length() > 0)
				hdrs.insert(_buf);
			_buf = std::move(line);
			break;
		case HeaderLineType::continuation:
			_buf += line;
			break;
		case HeaderLineType::end:
			if (_buf.length() > 0)
				hdrs.insert(_buf);
			_buf.clear();
			return (Status::done);
		}
	}
	return (Status::busy); // unreachable
}

HeaderParser::Status
HeaderParser::parse_cgi(std::iostream& ios, Headers& hdrs) {
	while (true) {
		utils::getline<"\n">(ios, _buf);
		if (_buf.length() == 0)
			return (Status::done);
		hdrs.update(_buf);
		_buf.clear();
	}
	return (Status::busy); // unreachable
}

static HeaderLineType
_getheaderline(std::iostream& ios, std::string& line) {
	utils::getline<"\r\n">(ios, line);
	if (line.size() == 0)		// bare CRLF
		return (HeaderLineType::end);
	if (http::is_ws(line[0]))	// leading whitespace
		return (HeaderLineType::continuation);
	return (HeaderLineType::first);
}