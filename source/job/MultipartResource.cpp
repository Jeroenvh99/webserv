#include "job/resource.hpp"
#include "logging/logging.hpp"

using job::MultipartResource;
using Elog = logging::ErrorLogger::Level;

static std::string	make_headers_and_body(route::Location const&);
static std::string	get_filename(http::BodyPart const&);

// Initializer methods

http::Status
MultipartResource::open(Job const& job, std::string const& boundary) {
	switch (job.request.method()) {
	case http::Method::POST:
		_parser = Parser(boundary);
		return (_post(job.location));
	default:
		return (http::Status::bad_request);
	}
}

void
MultipartResource::close() noexcept {
	_parser.clear();
	_directory = "";
	_iss.str("");
	_iss.clear(std::ios::eofbit);
}

// I/O methods

size_t
MultipartResource::read(webserv::Buffer& buf) {
	return (buf.get(_iss));
}

size_t
MultipartResource::write(webserv::Buffer const& buf) {
	try {
		Parser::Result	body_part = _parser.parse(buf);
		
		if (body_part) { // body part was processed; write to file
			std::ofstream	ofs(_directory / get_filename(*body_part));

			ofs << body_part->body;
			if (!ofs.good())
				throw (IOException("resource part not available for writing"));
		}
		return (buf.len());
	} catch (http::parse::Exception& e) {
		logging::elog.log(Elog::error, "Error when parsing multipart body: ", e.what(), ".");
		throw (IOException(e.what()));
	} catch (std::out_of_range&) {
		throw (IOException("Error when parsing multipart body: no file name was provided."));
	}
}

// Helper methods

http::Status
MultipartResource::_post(route::Location const& loc) {
	_directory = loc.to();
	if (!stdfs::exists(_directory) || !stdfs::is_directory(_directory))
		return (http::Status::internal_error);
	_iss.clear();
	_iss.str(make_headers_and_body(loc));
	return (http::Status::created);
}

// Helper functions

static std::string
make_headers_and_body(route::Location const& loc) {
	std::ostringstream	oss;
	std::string			representation = loc.from().c_str();

	oss << "Connection: keep-alive\r\n"
		<< "Content-Type: text/plain\r\n"
		<< "Content-Length: " << representation.length() << "\r\n"
		<< "Location: " << representation << "\r\n\r\n"
		<< representation;
	return (oss.str());
}

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