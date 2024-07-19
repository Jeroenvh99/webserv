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
		Response(Status = Status::ok);

		operator std::string() const;

		Status	status() const noexcept;
		Version	version() const noexcept;
		// Body	expects_body() const noexcept;

		void	init_from_headers();
		void	clear() noexcept;

	private:
		friend class Parser;

		static constexpr Version	_version = one_one;

		Status	_status;
	}; // class Response

	std::ostream&	operator<<(std::ostream&, Response const&);
}; // namespace http

#endif // HTTP_RESPONSE_HPP
