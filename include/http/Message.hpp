#ifndef HTTP_MESSAGE_HPP
# define HTTP_MESSAGE_HPP

# include "http.hpp"
# include "http/Body.hpp"

# include <string>
# include <unordered_map>
# include <utility>

namespace http {
	using Header = std::pair<std::string, std::string>;

	std::string	to_string(Header const&);
	Header		to_header(std::string const&);

	class Message {
	public:
		struct cmp {
			bool	operator()(std::string const&, std::string const&) const noexcept;
		}; // struct cmp
		using Headers = std::unordered_map<
			std::string,
			std::string,
			std::hash<std::string>,
			cmp>;

		operator std::string() const;

		Headers const&		headers() const noexcept;
		std::string const&	header(std::string const&) const;
		bool				has_header(std::string const&) const noexcept;
		size_t				header_count() const noexcept;
		Body				expects_body() const noexcept; // this is not very speedy

		void			header_add(Header&&);
		virtual void	clear() noexcept;

	private:
		friend class Parser;

		Headers	_headers;
	}; // class Message
}; // namespace http

#endif // HTTP_MESSAGE_HPP
