#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include "http.hpp"
# include "http/Message.hpp"
# include "http/Request.hpp"
# include "http/StatusCode.hpp"

# include <iostream>
# include <string>
# include <map>

namespace http
{
    class Response: public Message
    {
    public:
        Response(const Request &request);

		operator std::string() const;

		std::string const&	body() const noexcept;
		StatusCode			status() const noexcept;

    private:
        using MethodMap = std::map<Method, void (Response::*)(const Request&)>;

        StatusCode _code;
        HeaderMap _headers;

        std::string _body;

        void getMethod(const Request &request);
        
        static const inline MethodMap _methodMap = {
            {Method::GET, &Response::getMethod},
            // TODO: {RequestMethod::POST, &Response::postMethod},
            // TODO: {RequestMethod::DELETE, &Response::deleteMethod},
        };

        void readFromFile(const std::string &path);
    };

    std::ostream&	operator<<(std::ostream&, Response const&);
}

#endif // HTTP_RESPONSE_HPP
