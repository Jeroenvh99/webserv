#include <string>
#include <iostream>
#include <sstream>
#include <exception>

bool isHttpHeader(std::string& header) {
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

void parseHeader(std::string& headers) {
	std::istringstream s(headers);
	std::string temp;
	while (!s.eof()) {
		std::getline(s, temp);
		if (temp == "\r") {
			break;
		}
		if (!isHttpHeader(temp)) {
			throw std::exception();
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
		std::cout << "Name: " << name << " Value: " << value << "\n";
	}
	std::cout << std::endl;
}

// int main() {
// 	std::string in("Host: test.com\r\nContent-Type: app\r\n");
// 	try {
// 		parseHeader(in);
// 	} catch (std::exception &e) {
// 		std::cerr << "incorrect format for header\n";
// 	}
// }