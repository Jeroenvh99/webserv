#include "http/Message.hpp"

using http::Message;
using http::Header;

std::string
http::to_string(Header const& hdr) {
	return (hdr.first + ": " + hdr.second);
}

bool
Message::cmp::operator()(std::string const& s1, std::string const& s2) const noexcept {
	return (http::strcmp_nocase(s1, s2));
}
