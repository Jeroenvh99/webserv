#ifndef HTML_HPP
# define HTML_HPP

# include "http/Status.hpp"
# include <string>
# include <filesystem>

namespace html {
	std::string	error_page(http::Status);
	std::string	directory_list(std::filesystem::path const&);
}; // namespace html

#endif // HTML_HPP
