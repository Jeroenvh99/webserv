#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include "http.hpp"
# include "http/Message.hpp"
# include "http/Request.hpp"
# include "http/Status.hpp"

# include <iostream>
# include <string>
# include <map>

namespace http {
	class Response: public Message {
	public:
		Response(std::string const& = "", Status = Status::ok);
		Response(std::string&&, Status = Status::ok);
		Response(Status);

		operator std::string() const;

		std::string const&	body() const noexcept;
		Status				status() const noexcept;
		Version				version() const noexcept;
		HeaderMap const&	headers() const noexcept; // DB: see below

	private:
		static constexpr Version	_version = one_one;

		Status		_status;
		HeaderMap	_headers; // DB: is this necessary?
		std::string	_body;
	}; // class Response

	std::ostream&	operator<<(std::ostream&, Response const&);
}; // namespace http

#endif // HTTP_RESPONSE_HPP
