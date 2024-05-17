#include "http/Response.hpp"

using http::Response;

std::ostream&
http::operator<<(std::ostream& os, Response const&) {
	os << "<response goes here>";
	return (os);
}
