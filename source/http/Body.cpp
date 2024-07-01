#include "http/Body.hpp"

using http::Body;

Body::Body(Body::Type type, size_t length):
	_type(type), _length(length) {}

Body::operator bool() const noexcept {
	return (_type != Type::none);
}

Body::Type 
Body::type() const noexcept {
	return (_type);
}

size_t
Body::length() const noexcept {
	return (_length);
}