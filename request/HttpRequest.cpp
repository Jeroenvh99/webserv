#include "HttpRequest.hpp"

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

HttpRequest::HttpRequest() {}

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

void HttpRequest::parse(std::string& request) {
	RequestParseStatus status = RequestParseStatus::REQUESTLINE;
	int bodysize = 0;
	std::stringstream s(request);
	std::string requestline;
	std::stringstream copy(request);
	std::getline(copy, requestline);
	if (countWords(requestline) != 3) {
		throw HttpRequest::IncorrectRequestFormatException();
	}
	std::string temp;
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
	if (temp.substr(temp.length() - 2 ) != "\r\n") {
		throw std::exception();
	}
	temp.erase(temp.find_last_not_of(" \n\r\t") + 1);
	std::string versions[httpVersion::NO] = {"HTTP/1.0", "HTTP/1.1"};
	httpVersion versiontype[httpVersion::NO] = {httpVersion::ONE, httpVersion::ONEDOTONE};
	_version = httpVersion::NO;
	for (int i = 0; i < httpVersion::NO; i++) {
		if (temp == versions[i]) {
			_version = versiontype[i];
			break;
		}
	}
	if (_version == httpVersion::NO) {
		throw HttpRequest::IncorrectRequestFormatException();
	}

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
		if (value.back() != '\r') {
			throw HttpRequest::IncorrectHeaderFormatException();
		}
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
	if (!s.eof() || status != RequestParseStatus::END) {
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