#pragma once

#include <string>
#include <map>

#include "StatusCode.hpp"

using HeaderMap = std::map<std::string, std::string>;

class Response
{
private:
    StatusCode _code;
    HeaderMap _headers;
};