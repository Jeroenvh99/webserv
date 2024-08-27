#include "logging/logging.hpp"

#include <cstring>

using logging::ErrorLogger;

// Basic operations

ErrorLogger::ErrorLogger():
	Logger(std::cerr),
	_level(Level::error) {}

ErrorLogger::ErrorLogger(std::ostream& os, Level level):
	Logger(os),
	_level(level) {}

// Public methods

char const*
ErrorLogger::level_to_string(ErrorLogger::Level level) {
	switch (level) {
		case ErrorLogger::Level::debug:
			return ("debug");
		case ErrorLogger::Level::info:
			return ("info");
		case ErrorLogger::Level::notice:
			return ("notice");
		case ErrorLogger::Level::warning:
			return ("warning");
		case ErrorLogger::Level::error:
			return ("error");
		case ErrorLogger::Level::critical:
			return ("critical");
		case ErrorLogger::Level::alert:
			return ("alert");
		case ErrorLogger::Level::emergency:
			return ("emergency");
		default: // unreachable
			throw (std::invalid_argument(
				"level does not correspond to string"));
	}
}

ErrorLogger::Level
ErrorLogger::level_from_string(std::string const& that) {
	for (auto const& [level, string]: _levels)
		if (that == string)
			return (level);
	throw (std::invalid_argument(
		"string does not correspond to ErrorLogger::level"));
}
