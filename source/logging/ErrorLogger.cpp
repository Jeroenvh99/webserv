#include "logging/logging.hpp"

using logging::ErrorLogger;

// Basic operations

ErrorLogger::ErrorLogger():
	Logger(std::cerr),
	_level(default_level) {}

// Public methods

ErrorLogger::Level
ErrorLogger::level() const noexcept {
	return (_level);
}

ErrorLogger::Level&
ErrorLogger::level() noexcept {
	return (_level);
}

char const*
ErrorLogger::level_to_string(Level level) {
	switch (level) {
	case Level::debug:
		return ("debug");
	case Level::info:
		return ("info");
	case Level::notice:
		return ("notice");
	case Level::warning:
		return ("warning");
	case Level::error:
		return ("error");
	case Level::critical:
		return ("critical");
	case Level::alert:
		return ("alert");
	case Level::emergency:
		return ("emergency");
	default:
		__builtin_unreachable();
	}
}

ErrorLogger::Level
ErrorLogger::level_from_string(std::string const& that) {
	for (auto const& [level, literal]: literals)
		if (that == literal)
			return (level);
	throw (std::invalid_argument("string does not match error logger level"));
}
