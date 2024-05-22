#ifndef HTTP_MESSAGE_HPP
# define HTTP_MESSAGE_HPP

# include "http.hpp"

# include <string>
# include <unordered_map>
# include <utility>

namespace http {
	using Header = std::pair<std::string, std::string>;
	std::string	to_string(Header const&);

	class Message {
	public:
		struct cmp {
			bool	operator()(std::string const&, std::string const&) const noexcept;
		}; // struct Request::cmp
		using HeaderMap = std::unordered_map<
			std::string,
			std::string,
			std::hash<std::string>,
			cmp>;
	}; // class Message
}; // namespace http

#endif // HTTP_MESSAGE_HPP
