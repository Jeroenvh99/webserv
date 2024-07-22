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
		_open_builtin(_make_error_page(job.status));
}

void
Resource::open(job::RedirectionJob const& job) {
	_open_builtin(_make_redirection(job.destination));
}

size_t
Resource::write(webserv::Buffer const& buf) {
	if (_ofs.bad())
		throw (std::runtime_error("resource not available for writing"));
	size_t const	bytes = buf.put(_ofs);

	if (bytes == 0) { // assuming that an empty buffer signifies the end of client input
		_ofs.close();
		if (!_ifs.is_open())
			_status = Status::success;
	}
	return (bytes);
}

size_t
Resource::read(webserv::Buffer& buf) {
	if (_iss.eof()) { // read stringstream before filestream
		if (_ifs.bad())
			throw (std::runtime_error("resource not available for reading"));
		size_t const	bytes = buf.get(_ifs);

		if (_ifs.eof()) {
			_ifs.close();
			if (!_ofs.is_open())
				_status = Status::success;
		}
		return (bytes);
	}
	return (buf.get(_iss));
}

// Private modifiers

void
Resource::_open_ifile(stdfs::path const& pt) {
	_ifs.close();
	_ifs.open(pt);
	_status = Status::pending;
}

void
Resource::_open_ofile(stdfs::path const& pt) {
	_ofs.close();
	_ofs.open(pt);
	_status = Status::pending;
}

void
Resource::_open_builtin(std::string&& str) {
	_iss.str(str);
	_status = Status::pending;
}