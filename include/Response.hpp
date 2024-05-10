#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "StatusCode.hpp"
#include "HttpRequest.hpp"

class Response
{
public:
    Response(const HttpRequest &request);

    const std::string& get();

private:
    using HeaderMap = std::map<std::string, std::string>;
    using StatusMap = std::unordered_map<int, std::string>;

    StatusCode _code;
    HeaderMap _headers;

    std::string _body;
    std::string _response;

    static const inline StatusMap _statusMap = {
        {200, "OK"}
    };
};