#ifndef HTTP_STATUS_CODE_HPP
# define HTTP_STATUS_CODE_HPP

# include <array>
# include <string>
# include <utility>

namespace http {
	class StatusCode {
	public:
		enum Code: unsigned int {
			ok = 200,
			bad_request = 400,
			unauthorized = 401,
			forbidden = 403,
			not_found = 404,
			method_not_allowed = 405,
			internal_error = 500,
		}; // enum Code

		StatusCode(Code);

		operator std::string() const noexcept;

		char const*	description() const noexcept;

	private:
		using DescriptionMap = std::array<std::pair<Code, char const*>, 7>;
	
		static constexpr DescriptionMap	_descriptions = {{
			{Code::ok, "OK"},
			{Code::bad_request, "Bad Request"},
			{Code::unauthorized, "Unauthorized"},
			{Code::forbidden, "Forbidden"},
			{Code::not_found, "Not Found"},
			{Code::method_not_allowed, "Method Not Allowed"},
			{Code::internal_error, "Internal Server Error"},
		}};
		static constexpr char const*	_no_description = "";

		Code	_code;
	}; // class StatusCode
}; // namespace http

#endif // HTTP_STATUS_CODE_HPP
