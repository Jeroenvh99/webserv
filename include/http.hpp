#ifndef HTTP_HPP
# define HTTP_HPP

# include <array>
# include <cstdint>
# include <string>
# include <utility>

namespace http {
	// HTTP methods

	enum class Method {
		GET = 0x1 < 0,
		HEAD = 0x1 < 1,
		POST = 0x1 < 2,
		PUT = 0x1 < 3,
		DELETE = 0x1 < 4,
		CONNECT = 0x1 < 5,
		OPTIONS = 0x1 < 6,
		TRACE = 0x1 < 7,
	}; // enum class Method

	using MethodMap = std::array<std::pair<Method, char const*>, 8>;
	constexpr MethodMap	methods = {{
		{Method::GET, "GET"},
		{Method::HEAD, "HEAD"},
		{Method::POST, "POST"},
		{Method::PUT, "PUT"},
		{Method::DELETE, "DELETE"},
		{Method::CONNECT, "CONNECT"},
		{Method::OPTIONS, "OPTIONS"},
		{Method::TRACE, "TRACE"}
	}};

	Method		method_from_string(std::string const&);
	char const*	method_to_string(Method);

	// HTTP versions

	using Version = std::pair<uint8_t, uint8_t>;

	constexpr Version	one_zero(1, 0);
	constexpr Version	one_one(1, 1);

	Version		version_from_string(std::string const&);
	std::string	version_to_string(Version const&);
}; // namespace http

#endif // HTTP_HPP
