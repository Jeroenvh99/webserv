#include "Logger.hpp"

#include <sstream>

static char const*	dehash_level(ErrorLogger::Level);

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

		oss << "[" << dehash_level(level) << "]" << msg;
		static_cast<Logger const*>(this)->log(oss.str());
	}
}

static char const*
dehash_level(ErrorLogger::Level level) {
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