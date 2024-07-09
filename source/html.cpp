#include "html.hpp"

#include <sstream>

std::string
html::error_page(http::Status status) {
	std::ostringstream	oss;
	auto const			numstr = http::to_string(status);
	std::string const	desc = http::description(status);

	oss << "<!DOCTYPE html><html lang=\"en-US\"><head><meta charset=\"utf-8\" /><title>"
		<< numstr << ' ' << desc
		<< "</title></head><body><h1 align=\"center\">"
		<< numstr << ' ' << desc
		<< "</h1><hr /><p align=\"center\">webserv</p></body></html>";
	return (oss.str());
}

std::string
html::directory_list(stdfs::path const& path) {
	std::ostringstream	oss;

	oss << "!DOCTYPE html><html><meta charset=\"utf-8\" /><title>"
		<< std::string(path) << "</title></head><body>";
	for (auto const& entry: std::filesystem::directory_iterator(path))
		oss << "<p>" << entry.path() << "</p>";
	oss << "</body></html>";
	return (oss.str());
}
