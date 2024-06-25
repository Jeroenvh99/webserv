#include "http/Body.hpp"

Body::Body(Body::Type _type, size_t _length):
	type(_type), length(_length) {}

Body::operator bool() const noexcept {
	return (type != Type::none);
}