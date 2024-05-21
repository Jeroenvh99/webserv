#include "http.hpp"

#include <stdexcept>

namespace http {
	Method
	to_method(std::string const& that) {
		for (auto const& [method, string]: http::methods)
			if (that == string)
				return (method);
		throw (std::invalid_argument("unrecognized method"));
	}
	
	char const*
	to_string(Method that) {
		for (auto const& [method, string]: http::methods)
			if (that == method)
				return (string);
		throw (std::invalid_argument("unrecognized method")); // unreachable
	}
}; // namespace http
	
