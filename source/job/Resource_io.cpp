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

Resource::open(job::RedirectionJob const& job) {
	_open_builtin(_make_redirection(job.destination));
}

void
Resource::open(job::ErrorJob const& job) {
	if (job.file == Server::no_errpage
		|| _get_file(job.file) == http::Status::not_found)
		_open_builtin(_make_error_page(job.status));
}

void
Resource::close() noexcept {
	close_in();
	close_out();
}

void
Resource::close_in() noexcept {
	_iss.clear(std::ios::eofbit);
	_ifs.close();
}

void
Resource::close_out() noexcept {
	_ofs.close();
}

size_t
Resource::write(webserv::Buffer const& buf) {
	if (_ofs.bad())
		throw (IOException("resource not available for writing"));
	return (buf.put(_ofs));
}

size_t
Resource::read(webserv::Buffer& buf) {
	if (_iss.eof()) { // read stringstream before filestream
		if (_ifs.bad())
			throw (IOException("resource not available for reading"));
		return (buf.get(_ifs));
	}
	return (buf.get(_iss));
}

// Private modifiers

void
Resource::_open_ifile(stdfs::path const& pt) {
	_ifs.close();
	_ifs.open(pt);
}

void
Resource::_open_ofile(stdfs::path const& pt) {
	_ofs.close();
	_ofs.open(pt);
}

void
Resource::_open_builtin(std::string&& str) {
	if (str.length() > 0)
		_iss.clear(std::ios::goodbit);
	_iss.str(str);
}