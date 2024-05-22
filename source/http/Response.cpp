#include "http/Response.hpp"

#include <filesystem>
#include <fstream>

using http::Response;
using http::Request;
using http::StatusCode;

// Basic operations

Response::Response(std::string const& body, StatusCode status):
	_status(status),
	_body(body) {}

Response::Response(std::string&& body, StatusCode status):
	_status(status),
	_body(body) {}

Response::Response(Request const& request): 
    _status(StatusCode::ok),
    _body("") {
    (this->*_methodMap.at(request.method()))(request);
}

// Accessors

StatusCode
Response::status() const noexcept {
	return (_status);
}

std::string const&
Response::body() const noexcept {
	return (_body);
}

http::Version
Response::version() const noexcept {
	return (_version);
}

Response::HeaderMap const&
Response::headers() const noexcept {
	return (_headers);
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
        _status = StatusCode::not_found;
        // TODO: Load 404 page
        _body = "404";
    }
    else if (std::filesystem::is_directory(path))
    {
        // TODO: list directory
        _status = StatusCode::not_found;
        // TODO: Load 404 page
        _body = "404";
    }
    else
    {
        file.open(path);
        if (!file.is_open())
        {
            _status = StatusCode::forbidden;
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
