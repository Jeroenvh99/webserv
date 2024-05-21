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
		<< _uri << ' '
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

std::string const&
Request::uri() const noexcept {
	return (_uri);
}

std::string const&
Request::header(std::string const& name) const {
	auto const	it = _headers.find(name);

	if (it == _headers.end())
		throw (std::out_of_range("undefined header"));
	return (it->first);
}

bool
Request::has_header(std::string const& name) const noexcept {
	return (_headers.find(name) != _headers.end()); // C++20 has .contains()
}

std::string const&
Request::body() const noexcept {
	return (_body);
}

std::string&
Request::body() noexcept {
	return (_body);
}

// Modifiers

void
Request::clear() noexcept {
	_method = Method::GET;
	_version = Version(0, 0);
	_uri.clear();
	_headers.clear();
	_body.clear();
}

// Private methods
// Modifiers

void
Request::_header_add(Header&& hdr) {
	auto it = _headers.find(hdr.first);

	if (it != _headers.end()) {
		it->second.reserve(it->second.length() + 1 + hdr.second.length());
		it->second += ",";
		it->second += hdr.second;
	} else
		_headers.insert(hdr);	// DB: should I check the return value? The key is logically guaranteed to be unique.
}
