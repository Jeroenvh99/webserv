#ifndef HTTP_HEADER_HPP
# define HTTP_HEADER_HPP

# include <memory>

/* WIP */

namespace http {
	class Header {
	public:
		std::string const&				name();
		std::string const&				value();
		std::vector<std::string> const&	value_split();

		Header&	unset();
		Header&	value_append(std::string const&);
	private:
		using Impl = std::pair<std::string const, std::string>;

		Impl&										_impl;
		std::unique_ptr<std::vector<std::string>>	_split_cache;
	}; // class Header
}; // namespace http

#endif // HTTP_HEADER_HPP