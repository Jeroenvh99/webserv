#include "job/Resource.hpp"
#include "html.hpp"

using job::Resource;

std::string
Resource::_make_error_page(http::Status status) {
	std::ostringstream	oss;
	std::string			body = html::error_page(status);

	oss << "Content-Length: " << body.length() << "\r\n";
	oss << "\r\n" << body;
	return (oss.str());
}

std::string
Resource::_make_redirection(URI const& to) {
	std::string dest = to.path().string();
	if (to.query() != "")
		dest += "?" + to.query();
	if (to.fragment() != "")
		dest += "#" + to.fragment();
	return (std::string("Location: ") + dest +"\r\n\r\n");
}

std::string
Resource::_make_directory_list(stdfs::path const& pt) {
	std::ostringstream	oss;
	std::string			body = html::directory_list(pt);

	oss << "Content-Length: " << body.length() << "\r\n";
	oss << "\r\n" << body;
	return (oss.str());
}