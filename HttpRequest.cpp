#include "HttpRequest.hpp"

const char* HttpRequest::IncorrectRequestFormatException::what() const throw()
{
	return "httpException: Http request incorrectly formatted";
}

const char* HttpRequest::IncorrectHeaderFormatException::what() const throw()
{
	return "httpException: Http request header incorrectly formatted";
}

// when receiving 0 bytes from recv() parsing can begin
HttpRequest::HttpRequest(char *request) {
	if (request[0] != '\0') {
		std::string temp(request);
		_buffer.append(temp);
	}
	if (!_buffer.empty() && request[0] == '\0') {
		parse(_buffer);
	}
}

void HttpRequest::parse(std::string& request) {
	RequestParseStatus status = RequestParseStatus::REQUESTLINE;
	int bodysize = 0;
	std::stringstream s(request);
	std::string temp;
	// cannot use stream extraction and getline together
	std::getline(s, temp, ' ');
	std::string methods[requestType::NONE] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};
	requestType methodtype[requestType::NONE] = {requestType::GET, requestType::HEAD, requestType::POST, requestType::PUT, requestType::DELETE, requestType::CONNECT, requestType::OPTIONS, requestType::TRACE};
	_method = requestType::NONE;
	for (int i = 0; i < requestType::NONE; i++) {
		if (temp == methods[i]) {
			_method = methodtype[i];
			break;
		}
	}
	if (_method == requestType::NONE) {
		throw HttpRequest::IncorrectRequestFormatException();
	}
	std::getline(s, _requesturi, ' ');
	std::getline(s, temp, ' ');
	std::string versions[httpVersion::NONE] = {"HTTP/1.0", "HTTP/1.1"};
	httpVersion versiontype[httpVersion::NONE] = {httpVersion::ONE, httpVersion::ONEDOTONE};
	_version = httpVersion::NONE;
	for (int i = 0; i < httpVersion::NONE; i++) {
		if (temp == versions[i]) {
			_version = versiontype[i];
			break;
		}
	}
	if (temp.back() != '\r' or _method == httpVersion::NONE) {
		throw HttpRequest::IncorrectRequestFormatException();
	}
	status = RequestParseStatus::END;
	while (!s.eof()) {
		status = RequestParseStatus::HEADERS;
		std::getline(s, temp);
		if (temp == "\r") {
			status = RequestParseStatus::END;
			break;
		}
		if (!isHttpHeader(temp)) {
			throw HttpRequest::IncorrectHeaderFormatException();
		}
		std::stringstream h(temp);
		std::string name;
		name.erase(0, name.find_first_not_of(" \n\r\t"));
		name.erase(name.find_last_not_of(" \n\r\t") + 1);
		std::getline(h, name, ':');
		std::string value;
		std::getline(h, value, ':');
		value.erase(0, value.find_first_not_of(" \n\r\t"));
		value.erase(value.find_last_not_of(" \n\r\t") + 1);
		_headers.insert(std::make_pair(name, value));
		if (caseInsensitiveSearch(name, std::string("Content-Length"))) {
			_contentlength = std::stoi(value);
		}
	}
	while (!s.eof()) {
		status = RequestParseStatus::BODY;
		std::getline(s, temp);
		if (temp == "0\r" || bodysize == _contentlength) {
			status = RequestParseStatus::END;
			break;
		}
		bodysize += temp.length();
		_message += temp;
	}
	if (!(s.eof() && status == RequestParseStatus::END)) {
		throw HttpRequest::IncorrectRequestFormatException();
	}
}

bool caseInsensitiveCharCompare(char a, char b) {
	return std::tolower(a) == std::tolower(b);
}

bool caseInsensitiveSearch(const std::string& str, const std::string& substr) {
	auto it = std::search(str.begin(), str.end(), substr.begin(), substr.end(), caseInsensitiveCharCompare);
	return it != str.end();
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
