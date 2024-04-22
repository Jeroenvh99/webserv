#pragma once

#include <string>
#include <map>

#include "StatusCode.hpp"
#include "HttpRequest.hpp"

using HeaderMap = std::map<std::string, std::string>;

class Response
{
public:
    Response(const HttpRequest &request);
private:
    StatusCode _code;
    HeaderMap _headers;
};