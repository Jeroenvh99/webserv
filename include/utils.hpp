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

	template<StringLiteral DELIM>
	std::istream&	getline(std::istream&, std::string&);
	template<StringLiteral DELIM>
	std::istream&	getline_core(std::istream&, std::string&);
}; // namespace utils

# include "utils.tpp"

#endif // UTILS_HPP
