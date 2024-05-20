#include "http/Response.hpp"

#include <filesystem>
#include <fstream>

using http::Response;
using http::Request;
using http::StatusCode;

// Basic operations

Response::Response(const Request &request) : 
    _code(StatusCode::ok),
    _body("")
{
    (this->*_methodMap.at(request.method()))(request);
}

// Accessors

StatusCode
Response::status() const noexcept {
	return (_code);
}

std::string const&
Response::body() const noexcept {
	return (_body);
}

// Other methods

void http::Response::getMethod(const Request &request)
{
    readFromFile(request.uri());
}

void Response::readFromFile(const std::string &path)
{
    std::ifstream file;
    std::stringstream buffer;

    if (!std::filesystem::exists(path))
    {
        _code = StatusCode::not_found;
        // TODO: Load 404 page
        _body = "404";
    }
    else if (std::filesystem::is_directory(path))
    {
        // TODO: list directory
        _code = StatusCode::not_found;
        // TODO: Load 404 page
        _body = "404";
    }
    else
    {
        file.open(path);
        if (!file.is_open())
        {
            _code = StatusCode::forbidden;
            // TODO: Load 403 page
            _body = "403";
        }
        else
        {
            buffer << file.rdbuf();
            _body = buffer.str();
            file.close();
        }
    }
}
