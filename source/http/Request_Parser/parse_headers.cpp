#include "http/Request.hpp"

#include <sstream>

using http::Request;
using http::Header;
using Parser = Request::Parser;

enum class HeaderLineType {
	first,
	continuation,
	end,
	incomplete,
}; // enum class HeaderLineType

static HeaderLineType	_get_header_line(std::iostream&, std::string&);
static void				_header_flush(Request&, Header&&);
static void				_header_init(Header&, std::string&&);
static void				_header_continue(Header&, std::string&&);
static Parser::State	_parse_body_how(Request const& req, size_t&);
static bool				_get_body_length(size_t&, Request const&);

void
Parser::parse_headers(std::iostream& ios, Request& req) {
	std::string	line;

	while (true) {
		switch (_get_header_line(ios, line)) {
		case HeaderLineType::first:
			_header_flush(req, std::move(_tmp_hdr));
			_header_init(_tmp_hdr, std::move(line));
			break;
		case HeaderLineType::continuation:
			_header_continue(_tmp_hdr, std::move(line));
			break;
		case HeaderLineType::end:
			_header_flush(req, std::move(_tmp_hdr));
			_state = _parse_body_how(req, _body_length);
			return;
		case HeaderLineType::incomplete:
			throw (IncompleteLineException());
		}
	}
}

static HeaderLineType
_get_header_line(std::iostream& ios, std::string& line) {
	http::getline(ios, line);
	if (line.size() == 0)	// bare CRLF
		return (HeaderLineType::end);
	if (ios.eof()) {		// no CRLF
		ios << line;
		ios.clear();
		return (HeaderLineType::incomplete);
	} if (line[0] == ' ' || line[0] == '\t')	// leading whitespace
		return (HeaderLineType::continuation);
	return (HeaderLineType::first);
}

static std::string	_parse_key(std::istream&);

static void
_header_flush(Request& req, Header&& tmp) {
	if (tmp.first.size() == 0)	// undefined header
		return;
	req.header_add(tmp);
	tmp.first.clear();
	tmp.second.clear();
}

static void
_header_init(Header& tmp, std::string&& line) {
	std::istringstream	iss(line);

	tmp.first = _parse_key(iss);
	iss >> tmp.second;
	http::trim_ws(tmp.second);
}

static void
_header_continue(Header& tmp, std::string&& line) {
	tmp.second += ' ';
	tmp.second += http::trim_ws(line);
}

static std::string
_parse_key(std::istream& is) {
	std::string	s;

	std::getline(is, s, ':');
	if (is.eof())
		throw (Parser::HeaderException("bad header format"));
	return (s);
}

static Parser::State
_parse_body_how(Request const& req, size_t& body_length) {
	if (_get_body_length(body_length, req)) {
		if (req.has_header("Transfer-Encoding"))
			throw (std::invalid_argument("duplicate body length specification"));
		return (Parser::State::body_by_length);
	}
	try {
		std::string const&	strval = req.header("Transfer-Encoding");

		if (http::strcmp_nocase(strval, "chunked"))
			return (Parser::State::body_chunked);
		return (Parser::State::body_until_eof);
	} catch (std::out_of_range& e) {	// Transfer-Encoding is not defined
		return (Parser::State::done);
	}
}
static bool
_get_body_length(size_t& len, Request const& req) {
	try {
		std::string const&	strval = req.header("Content-Length");

		try {
			len = std::stoul(strval);
			return (true);
		} catch (std::out_of_range& e) {		// overflow
			throw (std::invalid_argument("bad header Content-Length"));
		} catch (std::invalid_argument& e) {	// non-numeric value
			throw (std::invalid_argument("bad header Content-Length"));
		}
	} catch (std::out_of_range& e) {	// Content-Length is not defined
		return (false);
	}
}
