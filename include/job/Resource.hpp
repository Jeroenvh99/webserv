
#ifndef JOB_RESOURCE_HPP
# define JOB_RESOURCE_HPP

# include "job.hpp"
# include "Buffer.hpp"
# include "http/Body.hpp"
# include "http/parse.hpp"

# include <fstream>
# include <optional>

namespace job {
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
		using MultipartOption = std::optional<MultipartParser>;

		http::Status	_get(route::Location const&);
		http::Status	_get_file(stdfs::path const&);
		http::Status	_get_directory(route::Location const&);
		http::Status	_get_directory_list(stdfs::path const&);
		http::Status	_post(route::Location const&);
		http::Status	_delete(route::Location const&);

		void	_open_ifile(stdfs::path const&);
		void	_open_ofile(stdfs::path const&);
		void	_open_builtin(std::string&&);

		std::string	_make_directory_list(stdfs::path const&);
		std::string	_make_redirection(URI const& to);
		std::string	_make_error_page(http::Status);

		MultipartOption		_multipart_parser;

		std::istringstream	_iss;
		std::ifstream		_ifs;
		std::ofstream		_ofs;
	}; // class Resource
}; // namespace job

#endif // JOB_RESOURCE_HPP