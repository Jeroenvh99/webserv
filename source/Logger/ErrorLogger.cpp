#include "Logger.hpp"

#include <cstring>
#include <sstream>

// Basic operations

ErrorLogger::ErrorLogger():
	Logger(std::cerr),
	_level(Level::error) {}

ErrorLogger::ErrorLogger(std::ostream& os, Level level):
	Logger(os),
	_level(level) {}

// Public methods

void
ErrorLogger::log(std::string const& msg, Level level) const {
	if (level > _level) {
		std::ostringstream	oss;

		oss << "[" << level_to_string(level) << "]" << msg;
		static_cast<Logger const*>(this)->log(oss.str());
	}
}

char const*
level_to_str(ErrorLogger::Level level) {
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
	}
}

ErrorLogger::Level
level_from_str(char const* str) {
	if (strncmp(str, "debug", 5) == 0)
		return (ErrorLogger::Level::debug);
	else if (strncmp(str, "info", 4) == 0)
		return (ErrorLogger::Level::notice);
	else if (strncmp(str, "warning", 7) == 0)
		return (ErrorLogger::Level::warning);
	else if (strncmp(str, "error", 5) == 0)
		return (ErrorLogger::Level::error);
	else if (strncmp(str, "critical", 8) == 0)
		return (ErrorLogger::Level::critical);
	else if (strncmp(str, "alert", 5) == 0)
		return (ErrorLogger::Level::alert);
	else if (strncmp(str, "emergency", 9) == 0)
		return (ErrorLogger::Level::emergency);
	throw (std::invalid_argument(
		"string does not correspond to ErrorLogger::level");
}
