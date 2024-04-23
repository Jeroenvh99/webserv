#include <string>
#include <sstream>
#include <iostream>

void parseBody(int contentlength, std::string& request) {
	int bodysize = 0;
	std::stringstream s(request);
	std::string message;
	std::string temp;
	while (!s.eof()) {
		std::getline(s, temp);
		if (!contentlength) {
			if (temp == "0\r") {
				std::getline(s, temp);
				if (temp == "\r") {
					break;
				}
			} else {
				std::getline(s, temp);
				temp += '\n';
				message += temp;
			}

		} else {
			temp += '\n';
			int i = 0;
			while (i + bodysize < contentlength && temp[i]) {
				message += temp[i];
				++i;
			}
			bodysize += i;
			if (bodysize == contentlength) {
				if (!s.eof() || temp[i] != '\0') {
					throw std::exception();
				}
				break;
			}
		}
	}
	std::cout << message;
}

int main() {
	std::string in("this is the body\r\n");
	try {
		parseBody(18, in);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for body\n";
	}
}