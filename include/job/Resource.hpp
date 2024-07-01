
#ifndef JOB_RESOURCE_HPP
# define JOB_RESOURCE_HPP

# include "job.hpp"
# include "Buffer.hpp"

# include <fstream>

namespace job {
	class Resource {
	public:
		Resource();
		~Resource();
		Resource(Resource const&) = delete;
		Resource(Resource&&);
		Resource&	operator=(Resource const&) = delete;
		Resource&	operator=(Resource&&);

		http::Status	open(Job const&);
		void			open(ErrorJob const&);
		void			close() noexcept;
		size_t			read(webserv::Buffer&);
		size_t			write(webserv::Buffer const&);
		Status			status() const noexcept;

	private:
		enum class Type;

		http::Status	_get(route::Location const&);
		http::Status	_get_file(stdfs::path const&);
		http::Status	_get_directory(route::Location const&);
		http::Status	_get_directory_list(stdfs::path const&);
		http::Status	_post(route::Location const&);
		http::Status	_delete(route::Location const&);

		void	_open_file(stdfs::path const&, std::ios::openmode);
		void	_open_builtin(std::string const&);
		void	_open_builtin(std::stringstream&&);

		Type					_type;
		Status					_status;
		union {
			std::fstream		_file;
			std::stringstream	_builtin;
		}; // anonymous union
	}; // class Resource

	enum class Resource::Type {
		none,
		file,
		builtin,
	}; // enum class Resource::Type
}; // namespace job

#endif // JOB_RESOURCE_HPP