#ifndef TIME_HPP
# define TIME_HPP

# include <time.h>

namespace webserv {
	struct Time {
		Time():
			tv(::time(nullptr)) {}

		operator time_t() const noexcept {
			return (tv);
		}

		time_t	tv;
	}; // struct Time
}; // namespace webserv

#endif // TIME_HPP
