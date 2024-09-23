#include "logging/logging.hpp"

using logging::ErrorLogger;

// Basic operations

ErrorLogger::ErrorLogger():
	Logger(std::cerr),
	_level(default_level) {}

// Public methods

char const*
ErrorLogger::level_to_string(Level level) {
	switch (level) {
	case debug:
		return ("debug");
	case info:
		return ("info");
	case notice:
		return ("notice");
	case warning:
		return ("warning");
	case error:
		return ("error");
	case critical:
		return ("critical");
	case alert:
		return ("alert");
	case emergency:
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
