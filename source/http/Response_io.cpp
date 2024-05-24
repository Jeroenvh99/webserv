#include "http/Response.hpp"

using http::Response;

std::ostream&
http::operator<<(std::ostream& os, Response const& resp) {
	os << to_string(resp.version()) << ' '
		<< to_string(resp.status()) << ' '
		<< description(resp.status()) << "\r\n"
		<< "Content-Length: " << std::to_string(resp.body().size()) << "\r\n"
		<< "Content-Type: text/html\r\n";
	for (auto const& hdr: resp.headers())
		os << to_string(hdr) << "\r\n";
	os << "\r\n" << resp.body();
	return (os);
}
