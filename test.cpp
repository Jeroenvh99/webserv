#include <iostream>
#include <string>
#include <sstream>

int main() {
	std::string str = "hello \nagain";
	std::stringstream s(str);
	std::string temp;
	s >> temp;
	std::cout << "the content of temp is: " << temp << "\n";
	std::getline(s, temp);
	std::cout << " second time: " << temp << " and the contents of the stream s are: " << s.str() << "\n";
	if (s.eof()) {
		std::cout << "the string is empty\n";
	}
	return 0;
}