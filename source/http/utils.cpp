#include "http/Request.hpp"

# include <algorithm>
# include <cstring>

std::istream&
http::getline(std::istream& is, std::string& str) {
	char	c;

	str.clear();
	while (is.get(c)) {
		str.push_back(c);
		if (str.size() >= 2
			&& str[str.size() - 1] == '\n'
			&& str[str.size() - 2] == '\r') { // C++20 has ends_with()
			str.erase(str.size() - 2);
			break;
		}
	}
	return (is);
}

std::string
http::trim_ws(std::string const& str) noexcept {
	std::string	res(str);

	return (ltrim_ws(rtrim_ws(res)));
}

std::string&
http::trim_ws(std::string& str) noexcept {
	return (ltrim_ws(rtrim_ws(str)));
}

std::string&
http::ltrim_ws(std::string& str) noexcept {
	str.erase(str.begin(), str.begin() + str.find_first_not_of(" \t"));
	return (str);
}

std::string&
http::rtrim_ws(std::string& str) noexcept {
	str.erase(str.find_last_not_of(" \t") + 1);
	return (str);
}

bool
http::strcmp_nocase(std::string const& s1, std::string const& s2) noexcept {
	return (std::equal(
		s1.begin(), s1.end(),
		s2.begin(), s2.end(),
		[](char c1, char c2) { return (std::tolower(c1) == std::tolower(c2)); }
	));
}

