#include "http/parse.hpp"

using http::parse::MultipartParser;

enum class MatchResult {full, possible, not};

// Basic operations

MultipartParser::MultipartParser(std::string const& boundary):
	_status(Status::begin),
	_boundary("--"s + boundary) {}

// Accessor methods

std::string const&
MultipartParser::boundary() const noexcept {
	return (_boundary)
}

void
MultipartParser::clear() noexcept {
	*this = MultipartParser();
}

// Whatchamacall this category methods

MultipartParser::Result
MultipartParser::parse(webserv::Buffer const& wsbuf) {
	_buf.clear();
	_buf << wsbuf;
	try {
		while (true) {
			switch (_status) {
			case Status::begin:
				parse_boundary()
				break;
			case Status::header:
				parse_headers();
				break;
			case Status::body:
				parse_body();
				break;
			case Status::end:
				return (_part);
			}
		}
	} catch (utils::IncompleteLineException&) {
		return (std::nullopt);
	}
}

void
MultipartParser::parse_boundary() {
	std::string	line;

	utils::getline<"\r\n">(_buf, line);
	if (rtrim_ws(line) != _boundary) // only trims HT and SP
		throw (parse::Exception("incorrect multipart boundary"));
}

void
MultipartParser::parse_headers() {
	std::string	line;

	while (true) {
		utils::getline<"\r\n">(_buf, line);
		if (line.length() == 0)
			return;
		_part.headers.insert(line);
	}
}

void
MultipartParser::parse_body() {
	auto	state = _buf.rdstate();
	auto	pos = _buf.tellg();

	std::string	body;
	
	try {
		utils::getline(_buf, _part.body, _boundary);
	} catch (IncompleteLineException& e) {
		_buf.clear(state);
		_buf.seekg(pos);
		throw (e);
	}
}

// Helper functions

static std::string	get_boundary(std::string const&);

std::optional<std::string>
http::parse::is_multipart(Request const& request) {
	try {
		std::string const&	content_type = *(request.headers().at("Content-Type").value().begin());

		if (!(content_type.starts_with("multipart/")))
			return (std::nullopt);
		return (find_boundary(content_type));
	} catch (std::out_of_range&) { // Content-Type is not defined
		return (std::nullopt);
	}
}

static std::string
get_boundary(std::string const& header_value) {
	std::string::size_type	begin = header_value.find("boundary=");
	
	if (begin == std::string::npos)
		throw (HeaderException("multipart content type does not define boundary"));
	begin += 9; // strlen("boundary=")
	return (header_value.substr(begin, header_value.find_first_of(" \t\r\n", begin))); // boundary is delimited by whitespace on the right side
	// Note: Quotation of the boundary string is NOT taken into account.
}