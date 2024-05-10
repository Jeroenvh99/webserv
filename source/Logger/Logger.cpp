#include "Logger.hpp"

#include <chrono>
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

using stdclock = std::chrono::system_clock;

void
Logger::timestamp() const {
	std::time_t	now = stdclock::to_time_t(stdclock::now());
	std::tm*	local_tm = std::localtime(&now);
	auto const	timezone = stdclock::from_time_t(now)
							- stdclock::from_time_t(std::mktime(std::gmtime(&now)));

	_os << std::put_time(local_tm, "[%Y/%m/%d:%H:%M:%S")
		<< std::showpos << std::put_time(local_tm, "%z]")
		<< ' ';
}
