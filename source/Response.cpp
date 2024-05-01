#include "Response.hpp"

#include <filesystem>

Response::Response(const HttpRequest &request)
{
    if (!std::filesystem::exists(request.getRequestUri()))
    {
        _code = StatusCode::NotFound;
        _body = "404";
        return;
    }

    _code = StatusCode::OK;
    _body = "Hello World!";
 }

 const std::string &Response::get()
 {
    return _response;
 }

 void Response::generate()
 {
    _response = "HTTP/1.1 ";
	_response += std::to_string(static_cast<int>(_code)) + " " + _statusMap.at(static_cast<int>(_code)) + "\r\n";
	_response += "Content-Length: " + std::to_string(_body.size()) + "\r\n";
	_response += "Content-Type: text/html\r\n\r\n";
	_response += _body;
 }
