#ifndef HTTP_STATUS_HPP
# define HTTP_STATUS_HPP

# include <array>
# include <string>
# include <utility>

namespace http {
	enum class Status: unsigned int {
		ok = 200,
		created = 201,
		bad_request = 400,
		unauthorized = 401,
		forbidden = 403,
		not_found = 404,
		method_not_allowed = 405,
		internal_error = 500,
	}; // enum Status

	std::string		to_string(Status) noexcept;
	unsigned int	to_uint(Status) noexcept;
	char const*		description(Status) noexcept;
	bool			is_client_error(Status) noexcept;
	bool			is_server_error(Status) noexcept;
	bool			is_error(Status) noexcept;

	using StatusDescriptionMap = std::array<std::pair<Status, char const*>, 8>;
	constexpr StatusDescriptionMap	_descriptions = {{
		{Status::ok, "OK"},
		{Status::created, "Created"},
		{Status::bad_request, "Bad Request"},
		{Status::unauthorized, "Unauthorized"},
		{Status::forbidden, "Forbidden"},
		{Status::not_found, "Not Found"},
		{Status::method_not_allowed, "Method Not Allowed"},
		{Status::internal_error, "Internal Server Error"},
	}};
}; // namespace http

#endif // HTTP_STATUS_HPP
