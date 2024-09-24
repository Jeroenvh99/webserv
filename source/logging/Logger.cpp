#include "logging/logging.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>

using logging::Logger;

// Basic operations

Logger::Logger(std::ostream& os):
	_os(&os) {}

Logger::~Logger() {
	detach_file();
}

// Public methods

void
Logger::attach_file(std::string const& file) {
	std::ostream*	fp = new std::ofstream(file);

	if (!fp->good()) {
		delete fp;
		throw (std::runtime_error("could not open log: " + file));
	}
	detach_file();
	_os = fp;
}

void
Logger::detach_file() noexcept {
	if (_os == &std::cout || _os == &std::cerr || _os == &std::clog)
		return;
	delete _os;
	_os = nullptr;
}

std::ostream&
Logger::os() noexcept {
	return (*_os); // if this ever turns out to be NULL, I'll eat my CPU...
}

std::string const&
Logger::timestamp() const noexcept {
	return (_timestamp);
}

void
Logger::timestamp_update() {
	using stdclock = std::chrono::system_clock;

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
