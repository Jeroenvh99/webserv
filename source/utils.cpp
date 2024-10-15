#include "utils/utils.hpp"

std::istream&
utils::getline(std::istream& is, std::string& str, std::string const& delim) {
	std::istream::iostate	state = is.rdstate();
	auto					pos = is.tellg();

	getline_core(is, str, delim);
	if (is.good())
		return (is);
	is.clear(state);
	is.seekg(pos);
	throw (IncompleteLineException());
}

std::istream&
utils::getline_core(std::istream& is, std::string& str, std::string const& delim) {
	char	c;

	str.clear();
	while (is.get(c)) {
		str.push_back(c);
		if (str.length() >= delim.length() && str.ends_with(delim)) {
			str.erase(str.length() - delim.length());
			break;
		}
	}
	return (is);
}