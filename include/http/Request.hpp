#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "http.hpp"
# include "http/Message.hpp"
# include "http/Body.hpp"
# include "URI.hpp"

# include <iostream>
# include <stdexcept>
# include <string>
# include <sstream>
# include <unordered_map>
# include <utility>

namespace http {
	class Request: public Message {
	public:
		Request(Method = Method::GET,
			Version = Version(0, 0),
			std::string const& = "");
		Request(Method, Version, URI&&);

		operator std::string() const;

		Method		method() const noexcept;
		Version		version() const noexcept;
		URI const&	uri() const noexcept;

		void	clear() noexcept;

	private:
		friend class Parser;

		Method	_method;
		Version	_version;
		URI		_uri;
	}; // class Request
}; // namespace http

#endif // HTTP_REQUEST_HPP
