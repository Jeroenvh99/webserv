#include "Logger.hpp"

#include <cstring>
#include <ctime>
#include <iomanip>

// Basic operations

Logger::Logger(std::ostream& os):
	_os(os) {}

// Public methods

void
Logger::log(std::string const& msg) const {
	timestamp();
	_os << msg << std::endl;
}

void
Logger::timestamp() const {
	std::time_t			now = std::time(nullptr);
	std::tm*			tm = std::localtime(&now);

	_os << std::put_time(tm, "[%Y/%m/%d:%H:%M:%S+0100]") << " "; // time zone; daylight savings!
}