#include "http/Request.hpp"

#include <iostream>

using http::Request;
using http::RequestMethod;
using http::HttpVersion;

const char* Request::IncorrectRequestFormatException::what() const throw()
{
	return "httpException: Http request incorrectly formatted";
}

const char* Request::IncorrectHeaderFormatException::what() const throw()
{
	return "httpException: Http request header incorrectly formatted";
}

int countWords(std::string& str) {
	int words = 0;
	for (size_t i = 0; i < str.size(); i++) {
		if (i > 0 && (std::isspace(str[i]) || str[i] == '\0') && !std::isspace(str[i - 1])) {
			++words;
		}
	}
	return words;
}

Request::Request(int contentlength) {
	_contentlength = contentlength;
	_method = RequestMethod::NONE;
	_version = HttpVersion::NO;
}

Request::Request(const Request& src) {
	*this = src;
}

Request &Request::operator=(const Request& src) {
	_method = src.getMethod();
	_version = src.getHttpVersion();
	_requesturi = src.getRequestUri();
	_headers = src.getHeaders();
	_message = src.getMessage();
	_contentlength = src.getContentLength();
	return *this;
}

// when receiving 0 bytes from recv() parsing can begin
// DB: No, it should begin sooner.
void Request::addBuffer(Buffer const& src) {
	if (src.len() == 0) {
		_contentlength = 0;
		parse(_buffer);
	}
	else {
		_buffer.reserve(_buffer.size() + src.len());
		for (auto const& c: src)
			_buffer += c;
	}
}

bool caseInsensitiveCharCompare(char a, char b) {
	return std::tolower(a) == std::tolower(b);
}

bool caseInsensitiveSearch(const std::string& str, const std::string& substr) {
	auto it = std::search(str.begin(), str.end(), substr.begin(), substr.end(), caseInsensitiveCharCompare);
	return it != str.end();
}

void Request::parse(std::string& request) {
	std::stringstream s(request);
	parseRequestLine(s);
	parseHeaders(s);
	parseBody(s);
	if (!s.eof()) {
		throw Request::IncorrectRequestFormatException();
	}
	std::cout << "Request at " << std::to_string(reinterpret_cast<uintptr_t>(this)) << " successfully parsed.\n"; // DB: replace with debug
}

void Request::parseRequestLine(std::stringstream &s) {
	std::string requestline;
	std::getline(s, requestline);
	if (countWords(requestline) != 3) {
		throw Request::IncorrectRequestFormatException();
	}
	std::string temp;
	std::stringstream s2(requestline);
	std::getline(s2, temp, ' ');
	_method = request_method_from_str(temp);
	std::getline(s2, _requesturi, ' ');
	std::getline(s2, temp, ' ');
	if (temp.back() != '\r') {
		throw Request::IncorrectRequestFormatException();
	}
	temp.erase(temp.find_last_not_of(" \n\r\t") + 1);
	std::string versions[HttpVersion::NO] = {"HTTP/1.0", "HTTP/1.1"};
	HttpVersion versiontype[HttpVersion::NO] = {HttpVersion::ONE, HttpVersion::ONEDOTONE};
	for (int i = 0; i < HttpVersion::NO; i++) {
		if (temp == versions[i]) {
			_version = versiontype[i];
			break;
		}
	}
	if (_version == HttpVersion::NO) {
		throw Request::IncorrectRequestFormatException();
	}
}

void Request::parseHeaders(std::stringstream &s) {
	std::string temp;
	while (!s.eof()) {
		std::getline(s, temp);
		if (temp == "\r") {
			return;
		}
		if (!isHttpHeader(temp)) {
			throw Request::IncorrectHeaderFormatException();
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
			throw Request::IncorrectHeaderFormatException();
		}
		value.erase(value.find_last_not_of(" \n\r\t") + 1);
		_headers.insert(std::make_pair(name, value));
		if (caseInsensitiveSearch(name, std::string("Content-Length"))) {
			_contentlength = std::stoi(value);
		}
	}
	throw Request::IncorrectRequestFormatException();
}

void Request::parseBody(std::stringstream &s) {
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
	throw Request::IncorrectRequestFormatException();
}

const RequestMethod &Request::getMethod() const {
	return _method;
}

const HttpVersion &Request::getHttpVersion() const {
	return _version;
}

const std::string &Request::getRequestUri() const {
	return _requesturi;
}

const Request::HeaderMap &Request::getHeaders() const {
	return _headers;
}

const std::string &Request::getMessage() const {
	return _message;
}

const int &Request::getContentLength() const {
	return _contentlength;
}

bool Request::isHttpHeader(std::string& header) {
	size_t colon = header.find(':');
	if (colon == std::string::npos) {
		return false;
	}
	for (size_t i = 0; i < colon; i++) {
		if (header[i] == ' ') {
			return false;
		}
	}
	bool hasvalue = false;
	for (size_t i = colon; i < header.length(); i++) {
		if (!std::isspace(header[i])) {
			hasvalue = true;
			break;
		}
	}
	return hasvalue;
}

Request::~Request() {}
