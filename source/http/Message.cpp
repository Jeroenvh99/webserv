#include "http/Message.hpp"

using http::Message;

bool
Message::cmp::operator()(std::string const& s1, std::string const& s2) const noexcept {
	return (http::strcmp_nocase(s1, s2));
}
