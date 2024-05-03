#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "StatusCode.hpp"
#include "HttpRequest.hpp"

namespace http
{
    class Response
    {
    public:
        Response(const HttpRequest &request);

        const std::string &get();

    private:
        using HeaderMap = std::map<std::string, std::string>;
        using StatusMap = std::unordered_map<int, std::string>;
        using MethodMap = std::map<requestType, void (Response::*)(const HttpRequest&)>;

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

        void getMethod(const HttpRequest &request);
        
        static const inline MethodMap _methodMap = {
            {requestType::GET, &Response::getMethod},
            // TODO: {requestType::POST, &Response::postMethod},
            // TODO: {requestType::DELETE, &Response::deleteMethod},
        };

        void readFromFile(const std::string &path);
    };
}