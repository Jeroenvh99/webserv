#ifndef UTILS_HPP
# define UTILS_HPP

# include <utility>

namespace utils {
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
}; // namespace utils

#endif // UTILS_HPP
