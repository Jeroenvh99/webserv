#include "http/Response.hpp"
#include "html.hpp"

using http::Response;
using http::Request;
using http::Status;

// Basic operations

Response::Response(Status status):
	_status(status),
	_body(default_html(status)) {}

Response::Response(std::string const& body, Status status):
	_status(status),
	_body(body) {}

Response::Response(std::string&& body, Status status):
	_status(status),
	_body(body) {}

// Accessors

Status
Response::status() const noexcept {
	return (_status);
}

std::string const&
Response::body() const noexcept {
	return (_body);
}

http::Version
Response::version() const noexcept {
	return (_version);
}

Response::HeaderMap const&
Response::headers() const noexcept {
	return (_headers);
}
