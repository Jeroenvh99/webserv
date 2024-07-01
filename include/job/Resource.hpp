
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
		Resource(Job const&);
		Resource(ErrorJob const&);
		Resource&	operator=(Resource const&) = delete;
		Resource&	operator=(Resource&&);

		void			close() noexcept;
		size_t			read(webserv::Buffer&);
		size_t			write(webserv::Buffer const&);
		StatusOption	status() const noexcept;

	private:
		enum class Type;

		StatusOption	_get(route::Location const&);
		StatusOption	_get_file(stdfs::path const&);
		StatusOption	_get_directory(route::Location const&);
		StatusOption	_get_directory_list(stdfs::path const&);
		StatusOption	_post(route::Location const&);
		StatusOption	_delete(route::Location const&);

		void	_open_file(stdfs::path const&, std::ios::openmode);
		void	_open_builtin(std::string const&);

		StatusOption			_status;
		Type					_type;
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