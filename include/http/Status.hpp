#ifndef HTTP_STATUS_HPP
# define HTTP_STATUS_HPP

# include <array>
# include <string>
# include <utility>

namespace http {
	enum class Status: unsigned int {
		ok = 200,
		created = 201,
		accepted = 202,
		no_content = 204,

		moved_permanently = 301,
		found = 302, // ?

		bad_request = 400,
		unauthorized = 401,
		forbidden = 403,
		not_found = 404,
		method_not_allowed = 405,
		not_acceptable = 406,
		length_required = 411,
		precondition_failed = 412,
		content_too_large = 413,
		too_many_requests = 429,

		internal_error = 500,
		not_implemented = 501,
		service_unavailable = 503,
		version_not_supported = 505,
	}; // enum Status

	std::string		to_string(Status) noexcept;
	unsigned int	to_uint(Status) noexcept;
	Status			to_status(std::string const&);
	char const*		description(Status) noexcept;
	bool			is_client_error(Status) noexcept;
	bool			is_server_error(Status) noexcept;
	bool			is_error(Status) noexcept;

	using StatusDescriptionMap = std::array<std::pair<Status, char const*>, 20>;
	constexpr StatusDescriptionMap	_descriptions = {{
		{Status::ok, "OK"},
		{Status::created, "Created"},
		{Status::accepted, "Accepted"},
		{Status::no_content, "No Content"},
		{Status::moved_permanently, "Resource has been moved, tough luck"},
		{Status::found, "Resource has been found, whatever that means"},
		{Status::bad_request, "Bad Request"},
		{Status::unauthorized, "Unauthorized"},
		{Status::forbidden, "Forbidden"},
		{Status::not_found, "Not Found"},
		{Status::method_not_allowed, "Method Not Allowed"},
		{Status::not_acceptable, "Not Acceptable"},
		{Status::length_required, "Length Required"},
		{Status::precondition_failed, "Precondition Failed"},
		{Status::content_too_large, "Content Too Large"},
		{Status::too_many_requests, "Too Many Requests"},
		{Status::internal_error, "Internal Server Error"},
		{Status::not_implemented, "Not Implemented"},
		{Status::service_unavailable, "Service Unavailable"},
		{Status::version_not_supported, "Version Not Supported"},
	}};
}; // namespace http

#endif // HTTP_STATUS_HPP
