#include "http/Request.hpp"

#include <sstream>
#include <iostream>

using http::Request;
using http::Method;
using http::Version;

// Basic operations

Request::Request(Method method, Version version, std::string const& uri):
	_method(method), _version(version), _uri(uri),
	_headers(), _body() {}

Request::Request(Method method, Version version, std::string&& uri):
	_method(method), _version(version), _uri(uri),
	_headers(), _body() {}

// Conversions

Request::operator std::string() const noexcept {
	std::ostringstream	oss;

	oss << to_string(_method) << ' '
		<< std::string(_uri) << ' '
		<< to_string(_version) << '\n';
	for (auto const& hdr: _headers)
		oss << to_string(hdr) << '\n';
	oss << "< body of size " << std::to_string(_body.size()) << " >";
	return (oss.str());
}

// Accessors

Method
Request::method() const noexcept {
	return (_method);
}

Version
Request::version() const noexcept {
	return (_version);
}

URI const&
Request::uri() const noexcept {
	return (_uri);
}

std::string const&
Request::header(std::string const& name) const {
	auto const	it = _headers.find(name);
 
	if (it == _headers.end())
		throw (std::out_of_range("undefined header"));
	return (it->second);
}

Request::HeaderMap const&
Request::headers() const noexcept {
	return (_headers);
}

bool
Request::has_header(std::string const& name) const noexcept {
	return (_headers.find(name) != _headers.end()); // C++20 has .contains()
}

size_t
Request::header_count() const noexcept {
	return (_headers.size());
}

static std::optional<size_t>	_get_body_length(Request const&);

Body
Request::has_body() const noexcept {
	auto	body_length = _get_body_length(*this);

	if (body_length) {
		if (*body_length)
			return ({Body::Type::by_length, *body_length});
		return ({Body::Type::none}); // Content-Length == 0
	}
	try {
		std::string const&	strval = req.header("Transfer-Encoding");

		if (http::strcmp_nocase(strval, "chunked")) // comma-separated values!
			return ({Body::Type::chunked});
		return ({Body::Type::none});
	} catch (std::out_of_range&) {	// Transfer-Encoding is not defined
		return ({Body::Type::none});
	}
}

static std::optional<size_t>
_get_body_length(Request const& req) {
	try {
		std::string const&	strval = req.header("Content-Length");

		try {
			return (std::stoul(strval));
		} catch (std::out_of_range& e) {		// overflow
			throw (Parser::HeaderException("bad header Content-Length"));
		} catch (std::invalid_argument& e) {	// non-numeric value
			throw (Parser::HeaderException("bad header Content-Length"));
		}
	} catch (std::out_of_range& e) {	// Content-Length is not defined
		return (std::nullopt_t);
	}
}

// Modifiers

void
Request::clear() noexcept {
	_method = Method::GET;
	_version = Version(0, 0);
	_uri = URI();
	_headers.clear();
	_body.clear();
}

// Private methods
// Modifiers

void
Request::_header_append(Header&& hdr) {
	auto it = _headers.find(hdr.first);

	if (it != _headers.end()) {
		it->second.reserve(it->second.length() + 1 + hdr.second.length());
		it->second += ",";
		it->second += hdr.second;
	} else
		_headers.insert(hdr);	// DB: should I check the return value? The key is logically guaranteed to be unique.
}
