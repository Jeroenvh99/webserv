#include "Client.hpp"

http::Request const&
ClientData::request() const noexcept {
	return (_request);
}

http::Request&
ClientData::request() noexcept {
	return (_request);
}

http::Request::Parser const&
ClientData::parser() const noexcept {
	return (_parser);
}

http::Request::Parser&
ClientData::parser() noexcept {
	return (_parser);
}

ClientData::State
ClientData::state() const noexcept {
	return (_state);
}
