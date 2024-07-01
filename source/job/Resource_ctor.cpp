#include "job/Resource.hpp"
#include "Server.hpp"
#include "html.hpp"

using job::Resource;

Resource::Resource():
	_type(Type::none),
	_status(Status::success) {}

Resource::~Resource() {
	close();
}

Resource::Resource(Resource&& that):
	_type(that._type),
	_status(that._status) {
	switch (_type) {
	case Type::file:
		new (&_file) std::fstream(std::move(that._file));
		break;
	case Type::builtin:
		new (&_builtin) std::stringstream(std::move(that._builtin));
		break;
	case Type::none: break;
	}
}

Resource&
Resource::operator=(Resource&& that) {
	if (this == &that)
		return (*this);
	close();
	_type = that._type;
	_status = that._status;
	switch (_type) {
	case Type::file:
		new (&_file) std::fstream(std::move(that._file));
		break;
	case Type::builtin:
		new (&_builtin) std::stringstream(std::move(that._builtin));
		break;
	case Type::none: break;
	}
	return (*this);
}