#include "http/Request.hpp"

#include <sstream>
#include <iostream>
#include <utility>

using http::Request;
using http::Method;
using http::Version;

// Basic operations

Request::Request(Method method, Version version, std::string const& uri):
	Message(),
	_method(method), _version(version), _uri(uri) {}

Request::Request(Method method, Version version, URI&& uri):
	Message(),
	_method(method), _version(version), _uri(std::move(uri)) {}

// Conversions

Request::operator std::string() const {
	std::ostringstream	oss;

	oss << to_string(_method) << ' '
		<< std::string(_uri) << ' '
		<< to_string(_version) << '\n';
	for (auto const& himpl: headers())
		oss << std::string(Header{himpl}) << '\n';

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

// Modifiers

void
Request::clear() noexcept {
	Message::clear();
	_method = Method::GET;
	_version = Version(0, 0);
	_uri = URI();
}