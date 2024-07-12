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

http::Body
Message::expects_body() const {
	auto	body_length = _get_body_length(*this);

	if (body_length) {
		if (_headers.contains("Transfer-Encoding", "chunked"))
			throw (parse::Exception("bad body description"));
		if (*body_length > 0)
			return {Body::Type::by_length, *body_length};
	}
	if (_headers.contains("Transfer-Encoding", "chunked"))
		return (Body::Type::chunked);
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
	using http::parse::HeaderException;

	try {
		http::Header const	hdr(msg.headers().at("Content-Length"));

		if (hdr.value().size() != 1)
			throw (HeaderException("Content-Length: can have only one value"));
		try {
			return (std::stoul(*(hdr.value().begin())));
		} catch (std::out_of_range&) {		// overflow
			throw (HeaderException("Content-Length: out of bounds"));
		} catch (std::invalid_argument&) {	// non-numeric value
			throw (HeaderException("Content-Length: bad number"));
		}
	} catch (std::out_of_range& e) {
		return (std::nullopt);
	}
}