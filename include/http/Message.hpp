#ifndef HTTP_MESSAGE_HPP
# define HTTP_MESSAGE_HPP

# include "http.hpp"
# include "http/Body.hpp"
# include "http/Header.hpp"

# include <string>

namespace http {
	class Message {
	public:
		operator std::string() const;

		Headers const&	headers() const noexcept;
		Headers&		headers() noexcept;

		Body	expects_body() const; // this is not very speedy

		virtual void	clear() noexcept;

	private:
		friend class Parser;

		Headers	_headers;
	}; // class Message
}; // namespace http

#endif // HTTP_MESSAGE_HPP
