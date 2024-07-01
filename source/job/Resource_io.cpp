#include "job/Resource.hpp"
#include "Server.hpp"
#include "html.hpp"

using job::Resource;

http::Status
Resource::open(job::Job const& job) {
	switch (job.request.method()) {
	case http::Method::GET:
		return (_get(job.location));
	case http::Method::POST:
		return (_post(job.location));
	case http::Method::DELETE:
		return (_delete(job.location));
	default:
		return (http::Status::not_implemented);
	}
}

void
Resource::open(job::ErrorJob const& job) {
	if (job.file == Server::no_errpage
		|| _get_file(job.file) == http::Status::not_found)
		_open_builtin(default_html(job.status));
}

void
Resource::close() noexcept {
	switch (_type) {
	case Type::file:
		_file.std::fstream::~fstream();
		break;
	case Type::builtin:
		_builtin.std::stringstream::~stringstream();
		break;
	case Type::none:
		break;
	}
	_type = Type::none;
	_status = Status::success;
}

size_t
Resource::write(webserv::Buffer const& buf) {
	switch (_type) {
	case Type::none:
		throw (std::runtime_error("cannot write to unopened resource"));
	case Type::file: {
		size_t const	bytes = buf.put(_file);

		if (_file.bad())
			_status = Status::failure;
		else if (bytes == 0) // assuming that an empty buffer signifies the end of client input
			_status = Status::success;
		return (bytes);
	} case Type::builtin:
		throw (std::runtime_error("cannot write to builtin resource"));
	}
	return (0); // unreachable
}

size_t
Resource::read(webserv::Buffer& buf) {
	switch (_type) {
	case Type::none:
		throw (std::runtime_error("cannot write to unopened resource"));
	case Type::file: {
		size_t const	bytes = buf.get(_file);

		if (_file.bad())
			_status = Status::failure;
		else if (_file.eof())
			_status = Status::success;
		return (bytes);
	} case Type::builtin:
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
	_status = Status::pending;
}

void
Resource::_open_builtin(std::string const& str) {
	close();
	_type = Type::builtin;
	new (&_builtin) std::stringstream(str);
	_status = Status::pending;
}

void
Resource::_open_builtin(std::stringstream&& ss) {
	close();
	_type = Type::builtin;
	new (&_builtin) std::stringstream(std::move(ss));
	_status = Status::pending;
}
