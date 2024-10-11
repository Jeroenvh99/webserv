
#ifndef JOB_RESOURCE_HPP
# define JOB_RESOURCE_HPP

# include "job.hpp"
# include "Buffer.hpp"
# include "http/Body.hpp"
# include "http/parse.hpp"

# include <fstream>
# include <optional>

namespace job {

class BaseResource {
public:
	class Exception;
	class IOException;

	virtual ~BaseResource() = default;

	static std::string	make_directory_list(stdfs::path const&);
	static std::string	make_redirection(URI const& to);
	static std::string	make_error_page(http::Status);
}; // class BaseResource

class BaseResource::Exception: public std::exception {
public:
	Exception();
	Exception(char const*);

	char const*	what() const noexcept;

private:
	char const*	_msg;
}; // class BaseResource::Exception

class BaseResource::IOException: public BaseResource::Exception {
public:
	IOException(char const*);
}; // class BaseResource::IOException

class Resource: public BaseResource {
public:
	http::Status	open(Job const&);
	void			open(job::RedirectionJob const& job);
	void			open(ErrorJob const&);
	void			close() noexcept;
	void			close_in() noexcept;
	void			close_out() noexcept;
	size_t			read(webserv::Buffer&);
	size_t			write(webserv::Buffer const&);

private:
	http::Status	_get(route::Location const&);
	http::Status	_get_file(stdfs::path const&);
	http::Status	_get_directory(route::Location const&);
	http::Status	_get_directory_list(stdfs::path const&);
	http::Status	_post(route::Location const&);
	http::Status	_delete(route::Location const&);

	void	_open_ifile(stdfs::path const&);
	void	_open_ofile(stdfs::path const&);
	void	_open_builtin(std::string&&);

	std::istringstream	_iss;
	std::ifstream		_ifs;
	std::ofstream		_ofs;
}; // class Resource

class MultipartResource: public BaseResource {
public:
	// Note: Much like CGI, multipart resources should really only yield their
	// HTTP status after all incoming data has been processed. However, for the
	// purposes of this project, returning 201 Created immediately if success
	// is _expected_ to be achieved should be fine. 

	http::Status	open(Job const&, std::string const&);
	void			close() noexcept;
	size_t			read(webserv::Buffer&);
	size_t			write(webserv::Buffer const&);

private:
	using Parser = http::parse::MultipartParser;

	http::Status	_post(route::Location const&);

	void	_open_ofile(stdfs::path const&);
	void	_open_builtin(std::string&&);

	Parser				_parser;
	stdfs::path			_directory;
	std::istringstream	_iss;
}; // class Resource

}; // namespace job

#endif // JOB_RESOURCE_HPP