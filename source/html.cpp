#include "html.hpp"

#include <sstream>

std::string
default_html(http::Status status) {
	std::ostringstream	oss("<!DOCTYPE html>");
	auto const			numstr = http::to_string(status);
	std::string const	desc = http::description(status);

	oss << "<html lang=\"en-US\"><head><meta charset=\"utf-8\" /><title>"
		<< numstr << ' ' << desc
		<< "</title></head><body><h1 align=\"center\">"
		<< numstr << ' ' << desc
		<< "</h1><hr /><p align=\"center\">webserv</p></body></html>";
	return (oss.str());
}
