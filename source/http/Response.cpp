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
	Message::clear();
	_status = http::Status::ok;
}

void
Response::init_from_headers() {
	try {
		_status = http::to_status(*(headers().at("Status").value().begin()));
	} catch (std::out_of_range&) {
		_status = http::Status::ok;
	}
}