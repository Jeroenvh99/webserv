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
static std::string		_get_key(std::istream&);

static void				_header_init(Header&, std::string&&);
static void				_header_continue(Header&, std::string&&);

void
Parser::_parse_headers(std::iostream& ios, Request& req) {
	std::string	line;

	while (true) {
		switch (_get_header_line(ios, line)) {
		case HeaderLineType::first:
			_header_add(req);
			_header_init(_tmp_hdr, std::move(line));
			break;
		case HeaderLineType::continuation:
			_header_continue(_tmp_hdr, std::move(line));
			break;
		case HeaderLineType::end:
			_header_add(req);
			_state = State::done;
			return;
		}
	}
}

void
Parser::_header_add(Request& req) {
	if (_tmp_hdr.first.length() == 0)	// attempting to add an undefined header
		return;
	req.header_add(std::move(_tmp_hdr));
	_tmp_hdr.first.clear();
	_tmp_hdr.second.clear();
}

static HeaderLineType
_get_header_line(std::iostream& ios, std::string& line) {
	Parser::getline(ios, line);
	if (line.size() == 0)	// bare CRLF
		return (HeaderLineType::end);
	if (http::is_ws(line[0]))	// leading whitespace
		return (HeaderLineType::continuation);
	return (HeaderLineType::first);
}

static void
_header_init(Header& tmp, std::string&& line) {
	std::istringstream	iss(line);

	tmp.first = _get_key(iss);
	iss >> tmp.second;
	http::trim_ws(tmp.second);
}

static void
_header_continue(Header& tmp, std::string&& line) {
	tmp.second += ' ';
	tmp.second += http::trim_ws(line);
}

static std::string
_get_key(std::istream& is) {
	std::string	s;

	std::getline(is, s, ':');
	if (is.eof())
		throw (Parser::HeaderException("bad header format"));
	return (s);
}
