#include "http/Message.hpp"
#include "http/parse.hpp"

#include <stdexcept>
#include <sstream>
#include <optional>

using http::Message;

static std::optional<size_t>	_get_body_length(Message const&);

bool
Message::cmp::operator()(std::string const& s1, std::string const& s2) const noexcept {
	return (http::strcmp_nocase(s1, s2));
}

// Conversions

Message::operator std::string() const {
	std::ostringstream	oss;

	for (auto const& hdr: _headers)
		oss << to_string(hdr) << '\n';
	return (oss.str());
}

// Accessors

Message::Headers const&
Message::headers() const noexcept {
	return (_headers);
}

std::string const&
Message::header(std::string const& name) const {
	auto const	it = _headers.find(name);
 
	if (it == _headers.end())
		throw (std::out_of_range("undefined header"));
	return (it->second);
}

bool
Message::has_header(std::string const& name) const noexcept {
	return (_headers.find(name) != _headers.end()); // C++20 has .contains()
}

size_t
Message::header_count() const noexcept {
	return (_headers.size());
}

http::Body // should be done differently: only Content-Length + Transfer-Encoding: chunked are incompatible 
Message::expects_body() const noexcept {
	auto	body_length = _get_body_length(*this);

	if (body_length) {
		if (*body_length)
			return {Body::Type::by_length, *body_length};
		return {Body::Type::none}; // Content-Length == 0
	}
	try {
		std::string const&	strval = header("Transfer-Encoding");

		if (http::strcmp_nocase(strval, "chunked")) // doesn't handle comma-separated values yet!
			return {Body::Type::chunked};
		return {Body::Type::none};
	} catch (std::out_of_range&) {
		return {Body::Type::none};
	}
}

// Modifiers

void
Message::header_add(Header&& hdr) {
	auto it = _headers.find(hdr.first);

	if (it != _headers.end()) {
		it->second.reserve(it->second.length() + 1 + hdr.second.length());
		it->second += ",";
		it->second += hdr.second;
	} else
		_headers.insert(hdr); // return value can be ignored; will always be unique
}

void
Message::clear() noexcept {
	_headers.clear();
}

// Non-member helpers

static std::optional<size_t>
_get_body_length(Message const& msg) {
	try {
		std::string const&	strval = msg.header("Content-Length");

		try {
			return (std::stoul(strval));
		} catch (std::out_of_range& e) {		// overflow
			throw (http::Parser::HeaderException("bad header Content-Length"));
		} catch (std::invalid_argument& e) {	// non-numeric value
			throw (http::Parser::HeaderException("bad header Content-Length"));
		}
	} catch (std::out_of_range& e) {	// Content-Length is not defined
		return (std::nullopt);
	}
}