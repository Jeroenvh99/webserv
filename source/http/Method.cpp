#include "http/http.hpp"
#include "http/parse.hpp"

namespace http {
	Method
	to_method(std::string const& that) {
		for (auto const& [method, string]: http::methods)
			if (that == string)
				return (method);
		throw (parse::MethodException("unrecognized method"));
	}
	
	char const*
	to_string(Method that) {
		for (auto const& [method, string]: http::methods)
			if (that == method)
				return (string);
		throw (parse::MethodException("unrecognized method")); // unreachable
	}
}; // namespace http
