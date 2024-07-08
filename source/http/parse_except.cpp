#include "http/parse.hpp"

using http::parse::Exception;
using http::parse::MethodException;
using http::parse::VersionException;
using http::parse::HeaderException;

Exception::Exception(char const* msg):
	_msg(msg) {}

char const*
Exception::what() const noexcept {
	return (_msg);
}

MethodException::MethodException(char const* msg):
	Exception(msg) {}

VersionException::VersionException(char const* msg):
	Exception(msg) {}

HeaderException::HeaderException(char const* msg):
	Exception(msg) {}
