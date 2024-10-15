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
		return (post(job.location));
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
	_parser.load(buf);
	try {
		write_core();
	} catch (http::parse::Exception& e) {
		logging::elog.log(Elog::error, "Error when parsing multipart body:");
		throw (IOException(e.what()));
	}
	return (buf.len());
}

// Helper methods

void
MultipartResource::write_core() {
	while (true) {
		Parser::Result	body_part = _parser.parse();
		
		if (!body_part) // more bytes are needed to read the entire body part
			break;
		try {
			std::ofstream	ofs(_directory / get_filename(*body_part));

			ofs << body_part->body;
			if (!ofs.good())
				throw (IOException("resource part not available for writing"));
		} catch (std::out_of_range&) { // don't sever the connection just yet
			logging::elog.log(Elog::warning, "Could not process multipart body: no file name was provided.");
		}
	}
}

http::Status
MultipartResource::post(route::Location const& loc) {
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
	std::string::size_type	end;

	if (begin == std::string::npos)
		throw (std::out_of_range("get_filename"));
	begin += 9; // strlen("filename=")
	if (disposition[begin] == '\"') {
		end = disposition.find('\"', begin + 1);
		if (end != std::string::npos)
			++begin;
	} else
		end = disposition.find(';', begin);
	return (disposition.substr(begin, end - begin));
}