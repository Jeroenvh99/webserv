#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include "http.hpp"
# include "StatusCode.hpp"
# include "Request.hpp"

# include <iostream>
# include <string>
# include <map>
# include <unordered_map>

namespace http
{
    class Response
    {
    public:
        Response(const Request &request);

        const std::string &get();

    private:
        using HeaderMap = std::map<std::string, std::string>;
        using StatusMap = std::unordered_map<int, std::string>;
        using MethodMap = std::map<Method, void (Response::*)(const Request&)>;

        StatusCode _code;
        HeaderMap _headers;

        std::string _body;
        std::string _response;

        static const inline StatusMap _statusMap = {
            {200, "OK"},
            {403, "Forbidden"},
            {404, "Not Found"},
            {405, "Method Not Allowed"},
            {500, "Internal Server Error"}
        };

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
