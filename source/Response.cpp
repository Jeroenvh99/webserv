#include "Response.hpp"

Response::Response(const HttpRequest &request)
{
    // if (request path does not exist)
    // {
    //     _code = StatusCode::NotFound;
    //     return;
    // }
    _code = StatusCode::OK;

    _body = "Hello World!";

    _response = "HTTP/1.1 ";
	_response += std::to_string(static_cast<int>(_code)) + " " + _statusMap.at(static_cast<int>(_code)) + "\r\n";
	_response += "Content-Length: " + std::to_string(_body.size()) + "\r\n";
	_response += "Content-Type: text/html\r\n\r\n";
	_response += _body;
 }

 const std::string &Response::get()
 {
    return _response;
 }
