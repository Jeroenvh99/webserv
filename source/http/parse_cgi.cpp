#include "http/parse.hpp"

std::optional<http::Headers::value_type>
http::parse_header_cgi(std::iostream& ios) {
	std::string	line;

	std::getline(ios, line);

	if (ios.eof()) {
		ios << line;
		return (std::nullopt);
	}
	return (http::Headers::parse(line));
}