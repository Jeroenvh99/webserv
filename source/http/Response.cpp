#include "http/Response.hpp"

#include <filesystem>
#include <fstream>

using http::Response;

Response::Response(const HttpRequest &request) : 
    _code(StatusCode::OK),
    _body("")
{
    (this->*_methodMap.at(request.getRequestType()))(request);
}

const std::string &Response::get()
{
    _response = "HTTP/1.1 ";
    _response += std::to_string(static_cast<int>(_code)) + " " + _statusMap.at(static_cast<int>(_code)) + "\r\n";
    _response += "Content-Length: " + std::to_string(_body.size()) + "\r\n";
    _response += "Content-Type: text/html\r\n\r\n";
    _response += _body;

    return _response;
}

void http::Response::getMethod(const HttpRequest &request)
{
    readFromFile(request.getRequestUri());
}

void Response::readFromFile(const std::string &path)
{
    std::ifstream file;
    std::stringstream buffer;

    if (!std::filesystem::exists(path))
    {
        _code = StatusCode::NotFound;
        // TODO: Load 404 page
        _body = "404";
    }
    else if (std::filesystem::is_directory(path))
    {
        // TODO: list directory
        _code = StatusCode::NotFound;
        // TODO: Load 404 page
        _body = "404";
    }
    else
    {
        file.open(path);
        if (!file.is_open())
        {
            _code = StatusCode::Forbidden;
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
