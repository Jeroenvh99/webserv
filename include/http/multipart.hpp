#ifndef HTTP_MULTIPART_HPP
# define HTTP_MULTIPART_HPP

# include "Buffer.hpp"

# include "http/Request.hpp"

# include <optional>
# include <sstream>
# include <string>

namespace http {

std::optional<std::string>	is_multipart(Request const&);

class MultipartParser {
public:
	using Result = std::optional<std::string>;

	std::string const&	boundary() const noexcept;
	std::string&		boundary() noexcept;
	Result				parse(webserv::Buffer const&);

private:
	std::string			_boundary_begin;
	std::string			_boundary_end;
	std::stringstream	_buf;
}; // class MultipartParser

}; // namespace http

#endif // HTTP_MULTIPART_HPP