#ifndef HTTP_HEADER_HPP
# define HTTP_HEADER_HPP

# include <memory>

namespace http {
	class Header {
	public:
		using Impl = std::pair<std::string const, std::string>;

		std::string const&				name;
		std::string const&				value;
		std::vector<std::string> const&	value_split();

		void	unset();
		void	value_append(std::string const&);
	private:
		Impl&										_impl;
		std::unique_ptr<std::vector<std::string>>	_split_cache;
	}; // class Header
}; // namespace http

#endif // HTTP_HEADER_HPP