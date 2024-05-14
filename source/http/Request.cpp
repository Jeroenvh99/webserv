#include "http/Request.hpp"

#include <sstream>

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

	oss << method_to_string(_method) << ' '
		<< _uri << ' '
		<< version_to_string(_version) << "\r\n";
	for (auto const& [key, value]: _headers)
		oss << key << ": " << value << "\r\n";
	oss << "\r\n";
	if (_body.size() > 0)
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
Request::header_add(Header const& header) {
	_headers.insert(header);
}

void
Request::header_add(Header&& header) {
	_headers.insert(header);
}

void
Request::header_add(std::string const& key, std::string const& value) {
	_headers.insert(std::make_pair(key, value));
}

void
Request::header_add(std::string&& key, std::string&& value) {
	_headers.insert(std::make_pair(key, value));
}

void
Request::clear() noexcept {
	_method = Method::GET;
	_version = Version(0, 0);
	_uri.clear();
	_headers.clear();
	_body.clear();
}

// cmp

bool
Request::cmp::operator()(std::string const& s1, std::string const& s2) const {
	return (strcmp_nocase(s1, s2));
}
