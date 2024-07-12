#include "http/Response.hpp"

using http::Response;

std::ostream&
http::operator<<(std::ostream& os, Response const& resp) {
	os << to_string(resp.version()) << ' '
		<< to_string(resp.status()) << ' '
		<< description(resp.status()) << "\r\n";
	if (resp.headers().size() > 0) {
		for (auto const& himpl: resp.headers())
			os << std::string(Header{himpl}) << "\r\n";
		os << "\r\n";
	}
	return (os);
}
