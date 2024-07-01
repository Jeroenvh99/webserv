#include "http/parse.hpp"

std::optional<http::Header>
http::parse_header_cgi(std::iostream& ios) {
	std::string	line;

	std::getline(ios, line);

	if (ios.eof()) {
		ios << line;
		return (std::nullopt);
	}
	return (http::to_header(line));
}