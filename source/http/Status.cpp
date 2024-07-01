#include "http/Status.hpp"

#include <sstream>

using http::Status;

std::string
http::to_string(Status status) noexcept {
	return (std::to_string(to_uint(status)));
}

unsigned int
http::to_uint(Status status) noexcept {
	return (static_cast<unsigned int>(status));
}

Status
http::to_status(std::string const& desc) {
	std::istringstream	iss(desc);
	unsigned int		num;

	iss >> num;
	return (static_cast<Status>(num));
}

char const*
http::description(Status status) noexcept {
	for (auto const& [key, desc]: http::_descriptions)
		if (status == key)
			return (desc);
	return ("");
}

bool
http::is_client_error(Status status) noexcept {
	unsigned int const	num = to_uint(status);

	return (num >= 400 && num <= 499);
}

bool
http::is_server_error(Status status) noexcept {
	unsigned int const	num = to_uint(status);

	return (num >= 500 && num <= 599);
}

bool
http::is_error(Status status) noexcept {
	return (is_client_error(status) || is_server_error(status));
}
