#include "HttpResponse.hpp"

const char* HttpResponse::InvalidResponseException::what() const throw()
{
	return "httpException: not enough information to create http response";
}

HttpResponse::HttpResponse(httpVersion version, int &status, HeaderMap &headers, std::string &message) {
	_version = version;
	_statuscode = status;
	switch (_statuscode) {
		case 200:
			_statustext = "Success";
			break;
		case 201:
			_statustext = "created";
			break;
		case 202:
			_statustext = "accepted";
			break;
		case 204:
			_statustext = "no content";
			break;
		case 301:
			_statustext = "moved permanently";
			break;
		case 302:
			_statustext = "found";
			break;
		case 303:
			_statustext = "See Other";
			break;
		case 304:
			_statustext = "not modified";
			break;
		case 400:
			_statustext = "bad request";
			break;
		case 401:
			_statustext = "unauthorized";
			break;
		case 403:
			_statustext = "forbidden";
			break;
		case 404:
			_statustext = "not found";
			break;
		case 405:
			_statustext = "method not allowed";
			break;
		case 408:
			_statustext = "request timeout";
			break;
		case 411:
			_statustext = "Length Required";
			break;
		case 413:
			_statustext = "Payload Too Large";
			break;
		case 414:
			_statustext = "uri too long";
			break;
		case 415:
			_statustext = "Unsupported Media Type";
			break;
		case 500:
			_statustext = "internal server error";
			break;
		case 501:
			_statustext = "not implemented";
			break;
		case 502:
			_statustext = "bad gateway";
			break;
		case 503:
			_statustext = "Service Unavailable";
			break;
		case 504:
			_statustext = "Gateway Timeout";
			break;
	}
	_headers = headers;
	_message = message;
	if (_headers.find("Content-Length") == _headers.end()) {
		_headers.insert(std::make_pair("Content-Length", std::to_string(_message.length())));
	}
}

HttpResponse::HttpResponse(const HttpResponse& src) {
	*this = src;
}

HttpResponse &HttpResponse::operator=(const HttpResponse& src) {
	if (this != &src)
	{
		_version = src.getHttpVersion();
		_statuscode = src.getStatusCode();
		_statustext = src.getStatusText();
		_headers = src.getHeaders();
		_message = src.getMessage();
	}
	return *this;
}

std::string HttpResponse::getResponse() {
	if (_version == httpVersion::NO || _statuscode < 200 || _message.empty()) {
		throw HttpResponse::InvalidResponseException();
	}
	_response += "HTTP\\";
	switch (_version) {
		case httpVersion::ONE:
			_response += "1 ";
			break;
		case httpVersion::ONEDOTONE:
			_response += "1.1 ";
			break;
	}
	_response += std::to_string(_statuscode) + " " + _statustext + "\r\n";
	if (_headers.size() > 0) {
		for (auto& h: _headers) {
			_response += h.first + ": " + h.second + "\r\n";
		}
	}
	_response += "\r\n";
	//use Content-length header for the body
	_response += _message + "\r\n";
	return _response;
}

httpVersion HttpResponse::getHttpVersion() const {
	return _version;
}

int HttpResponse::getStatusCode() const {
	return _statuscode;
}

std::string HttpResponse::getStatusText() const {
	return _statustext;
}

HttpResponse::HeaderMap HttpResponse::getHeaders() const {
	return _headers;
}

std::string HttpResponse::getMessage() const {
	return _message;
}

HttpResponse::~HttpResponse() {}