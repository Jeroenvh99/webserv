#include "http/Request.hpp"

#include <stdexcept>

namespace http {
	RequestMethod
	request_method_from_str(std::string const& that) {
		for (auto const& [method, string]: http::request_methods)
			if (that == string)
				return (method);
		throw (Request::IncorrectRequestFormatException());
	}
	
	char const*
	request_method_to_str(RequestMethod that) {
		for (auto const& [method, string]: http::request_methods)
			if (that == method)
				return (string);
		throw (Request::IncorrectRequestFormatException()); // unreachable
	}
}; // namespace http
	
