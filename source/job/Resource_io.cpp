#include "job/Resource.hpp"
#include "Server.hpp"

using job::Resource;
using Elog = logging::ErrorLogger::Level;

static std::string	get_filename(http::BodyPart const&);

http::Status
Resource::open(job::Job const& job) {
	switch (job.request.method()) {
	case http::Method::GET:
		return (_get(job.location));
	case http::Method::POST: {
		std::optional<std::string>	multipart_boundary = http::parse::is_multipart(job.request);

		if (multipart_boundary) {
			_multipart_parser = MultipartParser(*multipart_boundary);
			_multipart_directory = job.location.to();
			return(_post_multipart(job.location));
		}
		return (_post(job.location));
	}
	case http::Method::DELETE:
		return (_delete(job.location));
	default:
		return (http::Status::not_implemented);
	}
}

void
Resource::open(job::RedirectionJob const& job) {
	_open_builtin(_make_redirection(job.destination));
}

void
Resource::open(job::ErrorJob const& job) {
	if (job.file == VirtualServer::no_errpage
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
	_multipart_parser = std::nullopt;
	_multipart_directory = "";
	_ofs.close();
}

size_t
Resource::write(webserv::Buffer const& buf) {
	if (_multipart_parser)
		return (parse_multipart_and_write(buf));
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

size_t
Resource::parse_multipart_and_write(webserv::Buffer const& buf) {
	try {
		MultipartParser::Result	body_part = _multipart_parser->parse(buf);
		
		if (body_part) {
			std::string	fname = get_filename(*body_part);
			_open_ofile(_multipart_directory / fname);
			_ofs << body_part->body;
			if (!_ofs.good())
				throw (IOException("resource not available for writing"));
			_ofs.close();
		}
		return (buf.len());
	} catch (http::parse::Exception& e) {
		logging::elog.log(Elog::error, "Error when parsing multipart body: ", e.what(), ".");
		throw (IOException(e.what()));
	} catch (std::out_of_range&) {
		throw (IOException("Error when parsing multipart body: no file name was provided."));
	}
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

// Helper functions

static std::string
get_filename(http::BodyPart const& bpart) {
	std::string const		disposition = bpart.headers.at("Content-Disposition").csvalue();
	std::string::size_type	begin = disposition.find("filename=");

	if (begin == std::string::npos)
		throw (std::out_of_range("get_filename"));
	begin += 9; // strlen("filename=")

	std::string::size_type	end = disposition.find(';', begin);

	return (disposition.substr(begin, end));
}