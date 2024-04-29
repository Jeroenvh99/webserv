#include <iostream>
#include <string>
#include <vector>
#include "regexengine.hpp"
// argv[1] string to search in, argv[2] search pattern
// tokenize pattern to array with strings of allowed characters

int checkregex(std::string pattern) {
	for (int i = 0; i < pattern.length(); i++) {
		if (i == pattern.length() - 1 && pattern[i] == '\\') {
			return 0;
		}
	}
	return 1;
}

std::vector<std::string> patternToStrings(std::string pattern) {
	std::vector<std::string> patternArray;
regex_utils::pattern_state state = regex_utils::NORMAL;
	for (int i = 0; i < pattern.length(); i++) {
		if (state == regex_utils::BACKSLASH) {
			if (pattern[i] == 'd') {
				patternArray.push_back(std::string("0123456789"));
			} else if (pattern[i] == 'D') {
				std::string s;
				for (char c = 0; c < 127; c++) {
					if (c < '0' || c > '9') {
						s.push_back(c);
					}
				}
				patternArray.push_back(s);
			} else if (pattern[i] == '.') {
				patternArray.push_back(std::string("."));
			} else if (pattern[i] == 'w') {
				std::string s;
				for (char c = 0; c < 127; c++) {
					if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') {
						s.push_back(c);
					}
				}
				patternArray.push_back(s);
			} else if (pattern[i] == 'W') {
				std::string s;
				for (char c = 0; c < 127; c++) {
					if (c < 'A' && c > 'Z' && c < 'a' && c > 'z' && c < '0' && c > '9' && c != '_') {
						s.push_back(c);
					}
				}
				patternArray.push_back(s);
			} else if (pattern[i] == 's') {
				patternArray.push_back(std::string(" \t\n\r"));
			} else if (pattern[i] == 'S') {
				std::string s;
				for (char c = 0; c < 127; c++) {
					if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
						s.push_back(c);
					}
				}
				patternArray.push_back(s);
			}
			++i;
		} else if (state == regex_utils::SQUAREBRACKETS) {
			int allowed = 1;
			if (pattern[i] == '^') {
				allowed = 0;
				++i;
			}
			char from = pattern[i];
			char to = from;
			if (pattern[i + 1] == '-') {
				to = pattern[i + 2];
				i += 2;
			}
			std::string s;
			for (char c = 0; c < 127; c++) {
				if (!allowed && (c < from || c > to)) {
					s.push_back(c);
				}
				if (allowed && (c >= from && c <= to)) {
					s.push_back(c);
				}
			}
			patternArray.push_back(s);
			i += 2;
		}
		if (pattern[i] == '\\') {
			state = regex_utils::BACKSLASH;
		} else if (pattern[i] == '.') {
			std::string s;
			for (char c = 0; c < 127; c++) {
				s.push_back(c);
			}
			patternArray.push_back(s);
		} else if (pattern[i] == '[') {
			state = regex_utils::SQUAREBRACKETS;
		} else if (pattern[i]) {
			state = regex_utils::NORMAL;
			std::string s(1, pattern[i]);
			patternArray.push_back(s);
		}
	}
	return patternArray;
}

void searchPattern(std::string source, std::vector<std::string> pattern) {
	int match = 0;
	for (int i = 0; i < source.length(); i++) {
		if (pattern[0].find(source[i]) != -1) {
			match = 1;
			int j = 1;
			for (; j < pattern.size(); j++) {
				if (pattern[j].find(source[i + j]) == -1) {
					match = 0;
					break;
				}
			}
			if (match) {
				std::cout << source.substr(i, j) << std::endl;
			}
			i += j;
		} else {
			match = 0;
		}
	}
}

int regexsearch(std::vector<std::string> info) {
	if (!checkregex(info[1])) {
		std::cerr << "Syntax error in expression " << info[1] << std::endl;
		return 0;
	}
	std::vector<std::string> allowedChars = patternToStrings(info[1]);
	// for (std::string str : allowedChars) {
	// 	std::cout << " " << str << " ";
	// }
	// std::cout << std::endl;
	searchPattern(info[0], allowedChars);
	return 1;
}

// int main(int argc, char **argv) {
// 	if (argc < 3) {
// 		if (argc < 2) {
// 			std::cerr << "No string to search in" << std::endl;
// 		}
// 		std::cerr << "No pattern to search for" << std::endl;
// 		return 1;
// 	} else if (argc > 3) {
// 		std::cerr << "Too many arguments, I allow only one pattern" << std::endl;
// 		return 1;
// 	}
// 	std::vector<std::string> regexinfo;
// 	regexinfo.push_back(argv[1]);
// 	regexinfo.push_back(argv[2]);
// 	return !regexsearch(regexinfo);
// }