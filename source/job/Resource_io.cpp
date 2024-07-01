#include "job/Resource.hpp"

using job::Resource;

void
Resource::close() noexcept {
	switch (_type) {
	case Type::file:
		_file.std::fstream::~fstream();
		_type = Type::none;
		break;
	case Type::builtin:
		_builtin.std::stringstream::~stringstream();
		_type = Type::none;
		break;
	case Type::none:
		break;
	}
}

size_t
Resource::write(webserv::Buffer const& buf) {
	switch (_type) {
	case Type::none:
		throw (std::runtime_error("cannot write to unopened resource"));
	case Type::file:
		return (buf.put(_file));
	case Type::builtin:
		throw (std::runtime_error("cannot write to builtin resource"));
	}
	return (0); // unreachable
}

size_t
Resource::read(webserv::Buffer& buf) {
	switch (_type) {
	case Type::none:
		throw (std::runtime_error("cannot write to unopened resource"));
	case Type::file:
		return (buf.get(_file));
	case Type::builtin:
		return (buf.get(_builtin));
	}
	return (0); // unreachable
}

// Private modifiers

void
Resource::_open_file(stdfs::path const& pt, std::ios::openmode mode) {
	close();
	_type = Type::file;
	new (&_file) std::fstream(pt, mode);
}

void
Resource::_open_builtin(std::string const& str) {
	close();
	_type = Type::builtin;
	new (&_builtin) std::stringstream(str);
}
