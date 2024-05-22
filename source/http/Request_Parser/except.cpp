#include "http/Request.hpp"

using Parser = http::Request::Parser;

Parser::Exception::Exception(char const* msg):
	std::logic_error(msg) {}

Parser::StartLineException::StartLineException(char const* msg):
	Exception(msg) {}

Parser::MethodException::MethodException(char const* msg):
	StartLineException(msg) {}

Parser::VersionException::VersionException(char const* msg):
	StartLineException(msg) {}

Parser::HeaderException::HeaderException(char const* msg):
	Exception(msg) {}
