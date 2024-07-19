#ifndef HTML_HPP
# define HTML_HPP

# include "http/Status.hpp"
# include "route.hpp"
# include <string>

namespace html {
	std::string	error_page(http::Status);
	std::string	directory_list(stdfs::path const&);
}; // namespace html

#endif // HTML_HPP
