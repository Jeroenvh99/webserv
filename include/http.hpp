#ifndef HTTP_HPP
# define HTTP_HPP

# include <array>
# include <cstdint>
# include <string>
# include <utility>

namespace http {
	// Forward class declarations

	class Request;
	class Response;

	// HTTP methods

	enum class Method {
		GET = 0x1 << 0,
		HEAD = 0x1 << 1,
		POST = 0x1 << 2,
		PUT = 0x1 << 3,
		DELETE = 0x1 << 4,
		CONNECT = 0x1 << 5,
		OPTIONS = 0x1 << 6,
		TRACE = 0x1 << 7,
		NONE = 0x1 << 8,
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
	constexpr std::initializer_list<Method>	any_method = {
		Method::GET, Method::HEAD, Method::POST, Method::PUT,
		Method::DELETE, Method::CONNECT, Method::OPTIONS, Method::TRACE
	};

	Method		to_method(std::string const&);
	char const*	to_string(Method);

	// HTTP versions

	using Version = std::pair<uint8_t, uint8_t>;

	constexpr Version	one_zero(1, 0);
	constexpr Version	one_one(1, 1);

	Version		to_version(std::string const&);
	std::string	to_string(Version const&);

	std::istream&	getline(std::istream&, std::string&);
	bool			strcmp_nocase(std::string const&, std::string const&) noexcept;
	bool			is_ws(char c);
	std::string		trim_ws(std::string const&) noexcept;
	std::string&	trim_ws(std::string&) noexcept;
	std::string&	ltrim_ws(std::string&) noexcept;
	std::string&	rtrim_ws(std::string&) noexcept;
}; // namespace http

#endif // HTTP_HPP
