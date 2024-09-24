#include "logging/logging.hpp"

using logging::Variable;

Variable::Variable(Type type):
	_type(type), _data() {}

Variable::Variable(Type type, std::string const& data):
	_type(type), _data(data) {}

Variable::Variable(Type type, std::string&& data):
	_type(type), _data(data) {}

Variable::Variable(std::string const& data):
	_type(literal), _data(data) {}

Variable::Variable(std::string&& data):
	_type(literal), _data(data) {}

Variable::Type
Variable::type() const noexcept {
	return (_type);
}

std::string const&
Variable::data() const noexcept {
	return (_data);
}
