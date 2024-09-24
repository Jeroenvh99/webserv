#ifndef LOGGING_ERROR_LOGGER_TPP
# define LOGGING_ERROR_LOGGER_TPP

# ifndef LOGGING_HPP
#  error "include in logging.hpp"
# endif // LOGGING_HPP

# include <type_traits>

namespace logging {
	template<typename T>
	typename std::enable_if<std::is_integral<T>::value, std::string>::type
	to_string(T const& arg) {
		return (std::to_string(arg));
	}

	template<typename T>
	typename std::enable_if<std::is_floating_point<T>::value, std::string>::type
	to_string(T const& arg) {
		return (std::to_string(arg));
	}

	template<typename T>
	typename std::enable_if<!std::is_integral<T>::value, std::string>::type
	to_string(T const& arg) {
		return (std::string(arg));
	}

	template<typename... Ts>
	void
	ErrorLogger::log(Level level, Ts... args) {
		if (level < _level)
			return;
		timestamp_update();
		os() << "[" << level_to_string(level) << "] "
			<< "[" << timestamp() << "] ";
		(os() << ... << to_string(args));
		os() << std::endl;
	}
}; // namespace logging

#endif // LOGGING_ERROR_LOGGER_TPP
