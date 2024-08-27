#ifndef UTILS_TPP
# define UTILS_TPP

# ifndef UTILS_HPP
#  error "include in utils.hpp"
# endif // UTILS_HPP

namespace utils {
	template<StringLiteral DELIM>
	std::istream&
	getline(std::istream& is, std::string& str) {
		std::istream::iostate	state = is.rdstate();
		auto					pos = is.tellg();

		getline_core<DELIM>(is, str);
		if (is.good())
			return (is);
		is.clear(state);
		is.seekg(pos);
		throw (IncompleteLineException());
	}

	template<StringLiteral DELIM>
	std::istream&
	getline_core(std::istream& is, std::string& str) {
		char	c;

		str.clear();
		while (is.get(c)) {
			str.push_back(c);
			if (str.length() >= DELIM.length && str.ends_with(DELIM.value)) {
				str.erase(str.length() - DELIM.length);
				break;
			}
		}
		return (is);
	}
}; // namespace utils

#endif // UTILS_HPP
