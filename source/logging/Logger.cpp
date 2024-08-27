#include "logging/logging.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>

using logging::Logger;

// Basic operations

Logger::Logger(std::ostream& os):
	_os(os) {}

// Public methods

std::ostream&
Logger::os() const noexcept {
	return (_os);
}

std::string const&
Logger::timestamp() const noexcept {
	return (_timestamp);
}

using stdclock = std::chrono::system_clock;
void
Logger::timestamp_update() {
	std::ostringstream	updated;
	std::time_t			now = stdclock::to_time_t(stdclock::now());
	std::tm*			local_tm = std::localtime(&now);
	// auto const			timezone = stdclock::from_time_t(now)
	// 						- stdclock::from_time_t(std::mktime(std::gmtime(&now)));

	(void) timezone;
	_timestamp.clear();
	updated << std::put_time(local_tm, "%Y/%m/%d:%H:%M:%S")
		<< std::showpos << std::put_time(local_tm, "%z");
	_timestamp = updated.str();
} // this should be done without extra reallocations
