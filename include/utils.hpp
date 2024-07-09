#ifndef UTILS_HPP
# define UTILS_HPP

# include <algorithm>
# include <stdexcept>
# include <iostream>
# include <utility>

namespace utils {
	template<size_t N>
	struct StringLiteral {
		static constexpr size_t	size = N - 1;
		static constexpr size_t	length = N - 1;

		constexpr StringLiteral(char const (&str)[N]) {
			std::copy_n(str, N, value);
		}

		char	value[N];
	}; // struct StringLiteral

	struct StringHash {
		using is_transparent = void;

		std::size_t	operator()(char const* cstr) const {
			return (std::hash<std::string_view>{}(cstr));
		}

		std::size_t	operator()(std::string_view sv) const {
			return (std::hash<std::string_view>{}(sv));
		}

		std::size_t	operator()(std::string const& str) const {
			return (std::hash<std::string>{}(str));
		}
	}; // struct string_hash

	class IncompleteLineException {};

	template<utils::StringLiteral DELIM>
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

	template<utils::StringLiteral DELIM>
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
}; // namespace utils

#endif // UTILS_HPP
