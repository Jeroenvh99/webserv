#include "http/Response.hpp"

using http::Response;

std::ostream&
http::operator<<(std::ostream& os, Response const& resp) {
	StatusCode const	status = resp.status();
	std::string const&	body = resp.body();

	os << "HTTP/1.1 " << std::string(status) << ' ' << status.description() << "\r\n"
		<< "Content-Length: " << std::to_string(body.size()) << "\r\n"
		<< "Content-Type: text/html\r\n"
		<< "\r\n"
		<< body;
	return (os);
}
