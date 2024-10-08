#include "http/parse.hpp"

using http::parse::MultipartParser;

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
				parse_end();
				return (std::move(tmp));
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
	if (rtrim_ws(line) != _boundary) // checks HT and SP!
		throw (parse::Exception("incorrect multipart boundary"));
}

void
MultipartParser::parse_headers() {
	std::string	line;

	while (true) {
		utils::getline<"\r\n">(_buf, line);
		if (line.length() == 0)
			return;
		_tmp.headers.insert(line);
	}
}

void
MultipartParser::parse_body() {
	std::string	body;

	utils::getline<"\r\n">(_buf, body);

	auto		state = _buf.rdstate();
	auto		pos = _buf.tellg();
	std::string	next_boundary;

	_buf >> std::noskipws >> next_boundary;
	if (next_boundary.starts_with(_boundary)) {
		_tmp.body += body;
		_status = next_boundary.ends_with("--") ? Status::done : Status::headers;
	}
	is.clear(state);
	is.seekg(pos);
}

void
MultipartParser::parse_end() {

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