#pragma once

#include <string>
#include <unordered_map>

#include "StatusCode.hpp"

using HeaderMap = std::unordered_map<std::string, std::string>;

class Response
{
private:
    StatusCode code;
    HeaderMap headers;
};