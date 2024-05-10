#ifndef LOGGING_ERROR_LOGGER_TPP
# define LOGGING_ERROR_LOGGER_TPP

# ifndef LOGGING_HPP
#  error "include in logging.hpp"
# endif // LOGGING_HPP

namespace logging {
	template<typename... Ts>
	void
	ErrorLogger::log(Level level, Ts... ARGS) {
	if (level < _level)
		return;
	timestamp_update();
	os() << "[" << level_to_string(level) << "] "
		<< "[" << timestamp() << "] ";
	(os() << ... << ARGS);
	os() << std::endl;
	}
}; // namespace logging

#endif // LOGGING_ERROR_LOGGER_TPP
