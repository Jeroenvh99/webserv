#include "HttpResponse.hpp"

HttpResponse::HttpResponse(httpVersion version, int &status, HeaderMap &headers, std::string &message) {
	_version = version;
	_statuscode = status;
	switch (_statuscode) {
		case 200:
			_statustext = "Success";
			break;
	}
	_headers = headers;
	_message = message;
	if (_headers.find("Content-Length" == _headers.end())) {
		_headers.insert("Content-Length", _message.length().to_string())
	}
}

std::string HttpResponse::getResponse() {
	std::stringstream
}