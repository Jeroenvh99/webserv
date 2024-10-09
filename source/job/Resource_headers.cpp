#include "job/Resource.hpp"

#include <string>
#include <unordered_map>

using job::Resource;

static char const*	_get_mime_type(stdfs::path const&);

static const std::unordered_map<std::string, char const*>	mime_types = {
	{".json", "application/json"},
	{".pdf", "application/pdf"},
	{".xml", "application/xml"},
	{".zip", "application/zip"},
	{".jpg", "image/jpeg"},
	{".jpeg", "image/jpeg"},
	{".png", "image/png"},
	{".svg", "image/svg+xml"},
	{".ico", "image/vnd.microsoft.icon"},
	{".css", "text/css"},
	{".txt", "text/plain"},
	{".html", "text/html"},
	{".htm", "text/html"},
	{".js", "text/javascript"},
};

static char const*	default_mime_type = "application/octet_stream";

// Even better would be to read /etc/mime-types into a map and read that.
// Even more better would be to have UNIX `file` determine the file's type.
//   But that's probably not allowed.

std::string
Resource::make_headers_get(stdfs::path const& pt) {
	std::ostringstream	oss;

	oss << "Connection: keep-alive\r\n"
		<< "Content-Type: " << _get_mime_type(pt) << "\r\n"
		<< "Content-Length: " << std::filesystem::file_size(pt) << "\r\n";
	oss << "\r\n";
	return (oss.str());
}

std::string
Resource::make_headers_post(route::Location const& loc) {
	std::ostringstream	oss;

	oss << "Connection: keep-alive\r\n"
		<< "Content-Type: text/plain\r\n"
		<< "Content-Length: 38\r\n"
		<< "Location: " << loc.from().c_str() << "\r\n";
	oss << "\r\n";
	return (oss.str());
}

static char const*
_get_mime_type(stdfs::path const& pt) {
	if (!pt.has_extension())
		return (default_mime_type);
	try {
		return (mime_types.at(pt.extension().c_str()));
	} catch (std::out_of_range&) {
		return (default_mime_type);
	}
}