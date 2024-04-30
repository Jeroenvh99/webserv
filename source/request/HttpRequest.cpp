#include "HttpRequest.hpp"
#include <iostream>

const char* HttpRequest::IncorrectRequestFormatException::what() const throw()
{
	return "httpException: Http request incorrectly formatted";
}

const char* HttpRequest::IncorrectHeaderFormatException::what() const throw()
{
	return "httpException: Http request header incorrectly formatted";
}

int countWords(std::string& str) {
	int words = 0;
	for (int i = 0; i < str.size(); i++) {
		if (i > 0 && (std::isspace(str[i]) || str[i] == '\0') && !std::isspace(str[i - 1])) {
			++words;
		}
	}
	return words;
}

HttpRequest::HttpRequest(int contentlength) {
	_contentlength = contentlength;
	_method = requestType::NONE;
	_version = httpVersion::NO;
}

HttpRequest::HttpRequest(const HttpRequest& src) {
	*this = src;
}

HttpRequest &HttpRequest::operator=(const HttpRequest& src) {
	_method = src.getRequestType();
	_version = src.getHttpVersion();
	_requesturi = src.getRequestUri();
	_headers = src.getHeaders();
	_message = src.getMessage();
	_contentlength = src.getContentLength();
	return *this;
}

// when receiving 0 bytes from recv() parsing can begin
void HttpRequest::addBuffer(std::array<char, 512> request) {
	if (request[0] != '\0') {
		for (int i = 0; i < 512; i++) {
			_buffer += request[i];
		}
	}
	if (!_buffer.empty() && request[0] == '\0') {
		_contentlength = 0;
		parse(_buffer);
	}
}

bool caseInsensitiveCharCompare(char a, char b) {
	return std::tolower(a) == std::tolower(b);
}

bool caseInsensitiveSearch(const std::string& str, const std::string& substr) {
	auto it = std::search(str.begin(), str.end(), substr.begin(), substr.end(), caseInsensitiveCharCompare);
	return it != str.end();
}

void HttpRequest::parse(std::string& request) {
	std::stringstream s(request);
	parseRequestLine(s);
	parseHeaders(s);
	parseBody(s);
	if (!s.eof()) {
		throw HttpRequest::IncorrectRequestFormatException();
	}
}

void HttpRequest::parseRequestLine(std::stringstream &s) {
	std::string requestline;
	std::getline(s, requestline);
	if (countWords(requestline) != 3) {
		throw HttpRequest::IncorrectRequestFormatException();
	}
	std::string temp;
	std::stringstream s2(requestline);
	std::getline(s2, temp, ' ');
	std::string methods[requestType::NONE] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};
	requestType methodtype[requestType::NONE] = {requestType::GET, requestType::HEAD, requestType::POST, requestType::PUT, requestType::DELETE, requestType::CONNECT, requestType::OPTIONS, requestType::TRACE};
	for (int i = 0; i < requestType::NONE; i++) {
		if (temp == methods[i]) {
			_method = methodtype[i];
			break;
		}
	}
	if (_method == requestType::NONE) {
		throw HttpRequest::IncorrectRequestFormatException();
	}
	std::getline(s2, _requesturi, ' ');
	std::getline(s2, temp, ' ');
	if (temp.back() != '\r') {
		throw HttpRequest::IncorrectRequestFormatException();
	}
	temp.erase(temp.find_last_not_of(" \n\r\t") + 1);
	std::string versions[httpVersion::NO] = {"HTTP/1.0", "HTTP/1.1"};
	httpVersion versiontype[httpVersion::NO] = {httpVersion::ONE, httpVersion::ONEDOTONE};
	for (int i = 0; i < httpVersion::NO; i++) {
		if (temp == versions[i]) {
			_version = versiontype[i];
			break;
		}
	}
	if (_version == httpVersion::NO) {
		throw HttpRequest::IncorrectRequestFormatException();
	}
}

void HttpRequest::parseHeaders(std::stringstream &s) {
	std::string temp;
	while (!s.eof()) {
		std::getline(s, temp);
		if (temp == "\r") {
			return;
		}
		if (!isHttpHeader(temp)) {
			throw HttpRequest::IncorrectHeaderFormatException();
		}
		std::stringstream h(temp);
		std::string name;
		std::getline(h, name, ':');
		name.erase(0, name.find_first_not_of(" \n\r\t"));
		name.erase(name.find_last_not_of(" \n\r\t") + 1);
		std::string value;
		std::getline(h, value, ':');
		value.erase(0, value.find_first_not_of(" \n\r\t"));
		if (value.back() != '\r') {
			throw HttpRequest::IncorrectHeaderFormatException();
		}
		value.erase(value.find_last_not_of(" \n\r\t") + 1);
		_headers.insert(std::make_pair(name, value));
		if (caseInsensitiveSearch(name, std::string("Content-Length"))) {
			_contentlength = std::stoi(value);
		}
	}
	throw HttpRequest::IncorrectRequestFormatException();
}

void HttpRequest::parseBody(std::stringstream &s) {
	std::string temp;
	int bodysize = 0;
	while (!s.eof()) {
		std::getline(s, temp);
		temp += '\n';
		if ((_contentlength > 0 && temp == "0\r\n") || bodysize == _contentlength) {
			return;
		}
		bodysize += temp.length();
		_message += temp;
	}
	throw HttpRequest::IncorrectRequestFormatException();
}

const requestType &HttpRequest::getRequestType() const {
	return _method;
}

const httpVersion &HttpRequest::getHttpVersion() const {
	return _version;
}

const std::string &HttpRequest::getRequestUri() const {
	return _requesturi;
}

const HttpRequest::HeaderMap &HttpRequest::getHeaders() const {
	return _headers;
}

const std::string &HttpRequest::getMessage() const {
	return _message;
}

const int &HttpRequest::getContentLength() const {
	return _contentlength;
}

bool HttpRequest::isHttpHeader(std::string& header) {
	int colon = header.find(':');
	if (colon == std::string::npos) {
		return false;
	}
	for (int i = 0; i < colon; i++) {
		if (header[i] == ' ') {
			return false;
		}
	}
	bool hasvalue = false;
	for (int i = colon; i < header.length(); i++) {
		if (!std::isspace(header[i])) {
			hasvalue = true;
			break;
		}
	}
	return hasvalue;
}

const HttpRequest::HeaderMap &HttpRequest::getHeaders() const
{
	return _headers;
}

HttpRequest::~HttpRequest() {}
