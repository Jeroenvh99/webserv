#include "http/Message.hpp"
#include "http/parse.hpp"

#include <stdexcept>
#include <sstream>
#include <optional>

using http::Message;

static std::optional<size_t>	_get_body_length(Message const&);

// Conversions

Message::operator std::string() const {
	std::ostringstream	oss;

	for (auto const& himpl: _headers)
		oss << std::string(Header(himpl)) << '\n';
	return (oss.str());
}

// Accessors

http::Headers const&
Message::headers() const noexcept {
	return (_headers);
}

http::Headers&
Message::headers() noexcept {
	return (_headers);
}

http::Body // should be done differently: only Content-Length + Transfer-Encoding: chunked are incompatible 
Message::expects_body() const noexcept {
	auto	body_length = _get_body_length(*this);

	if (body_length) {
		if (*body_length)
			return {Body::Type::by_length, *body_length};
		return {Body::Type::none}; // Content-Length == 0
	}
	else if (_headers.contains("Transfer-Encoding", "chunked"))
		return (Body::Type::to_dechunk);
	return (Body::Type::none);
}

// Modifiers

void
Message::clear() noexcept {
	_headers.clear();
}

// Non-member helpers

static std::optional<size_t>
_get_body_length(Message const& msg) {
	try {
		http::Header const	hdr(msg.headers().at("Content-Length"));

		if (hdr.value().size() != 1)
			throw (http::Parser::HeaderException("bad header Content-Length"));
		try {
			return (std::stoul(*(hdr.value().begin())));
		} catch (std::out_of_range& e) {		// overflow
			throw (http::Parser::HeaderException("bad header Content-Length"));
		} catch (std::invalid_argument& e) {	// non-numeric value
			throw (http::Parser::HeaderException("bad header Content-Length"));
		}
	} catch (std::out_of_range& e) {	// Content-Length is not defined
		return (std::nullopt);
	}
}