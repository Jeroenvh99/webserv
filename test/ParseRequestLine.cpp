#include <iostream>
#include <exception>
#include "HttpRequest.hpp"

int countWords(std::string& str) {
	int words = 0;
	for (int i = 0; i < str.size(); i++) {
		if (i > 0 && (std::isspace(str[i]) || str[i] == '\0') && !std::isspace(str[i - 1])) {
			++words;
		}
	}
	return words;
}

void parseRequestLine(std::string& request) {
	RequestParseStatus status = RequestParseStatus::REQUESTLINE;
	int bodysize = 0;
	std::stringstream s(request);
	std::string requestline;
	std::stringstream copy(request);
	std::getline(copy, requestline);
	if (countWords(requestline) != 3) {
		throw std::exception();
	}
	std::string temp;
	std::getline(s, temp, ' ');
	std::string methods[requestType::NONE] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};
	requestType methodtype[requestType::NONE] = {requestType::GET, requestType::HEAD, requestType::POST, requestType::PUT, requestType::DELETE, requestType::CONNECT, requestType::OPTIONS, requestType::TRACE};
	requestType method = requestType::NONE;
	for (int i = 0; i < requestType::NONE; i++) {
		if (temp == methods[i]) {
			method = methodtype[i];
			break;
		}
	}
	if (method == requestType::NONE) {
		throw std::exception();
	}
	std::string requesturi;
	std::getline(s, requesturi, ' ');
	std::getline(s, temp, ' ');
	if (temp.substr(temp.length() - 2 ) != "\r\n") {
		throw std::exception();
	}
	temp.erase(temp.find_last_not_of(" \n\r\t") + 1);
	std::string versions[httpVersion::NO] = {"HTTP/1.0", "HTTP/1.1"};
	httpVersion versiontype[httpVersion::NO] = {httpVersion::ONE, httpVersion::ONEDOTONE};
	httpVersion version = httpVersion::NO;
	for (int i = 0; i < httpVersion::NO; i++) {
		if (temp == versions[i]) {
			version = versiontype[i];
			break;
		}
	}
	if (version == httpVersion::NO) {
		throw std::exception();
	}
	std::cout << "Method: " << method << " Uri: " << requesturi << " Httpversion: " << version << "\n";
}

// int main() {
// 	std::string in("GET www.test.com HTTP/1.0\r\n");
// 	try {
// 		parseRequestLine(in);
// 	} catch (std::exception &e) {
// 		std::cerr << "incorrect format for requestline\n";
// 	}
// }