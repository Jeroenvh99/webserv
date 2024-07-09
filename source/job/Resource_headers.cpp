#include "job/Resource.hpp"

using job::Resource;

std::string
Resource::_make_headers(stdfs::path const& pt) {
	std::ostringstream	oss;

	oss << "Content-Type: " << "text/html" << "\r\n"
		<< "Content-Length: " << std::filesystem::file_size(pt) << "\r\n";
	oss << "\r\n";
	return (oss.str());
}