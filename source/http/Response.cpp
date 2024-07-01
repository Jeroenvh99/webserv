#include "http/Response.hpp"
#include "http/parse.hpp"

using http::Response;
using http::Status;

// Basic operations

Response::Response(Status status):
	_status(status) {}

// Accessors

Status
Response::status() const noexcept {
	return (_status);
}

http::Version
Response::version() const noexcept {
	return (_version);
}

// Modifiers

void
Response::clear() noexcept {
	static_cast<Message*>(this)->clear();
	_status = http::Status::ok;
}

void
Response::init_from_headers() {
	try {
		_status = http::to_status(header("Status"));
	} catch (std::out_of_range&) {
		throw (http::Parser::Exception("CGI did not indicate response status"));
	}
}