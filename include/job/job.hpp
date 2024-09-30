#ifndef JOB_HPP
# define JOB_HPP

# include "webserv.hpp"
# include "Environment.hpp"
# include "http/Status.hpp"
# include "route.hpp"

# include <stdexcept>

namespace job {
	struct Job {
		Job(Client const&, Server const&, VirtualServer const&);

		bool	is_cgi() const noexcept;

		http::Request const&	request;
		VirtualServer const&	vserver;
		Server const&			server;
		route::Location const	location;
		Environment				environment;
	}; // struct Job
	int		is_httpredirect(job::Job const& job);

	struct RedirectionJob {
		RedirectionJob(job::Job const& todo, int redirindex);
		URI const	destination;
		const bool	permanent;
	};

	struct ErrorJob {
		ErrorJob(http::Status, VirtualServer const&);
		ErrorJob(http::Status, Job const&);

		http::Status const	status;
		stdfs::path const&	file;
	}; // struct ErrorJob

	class BaseResource {
	public:
		class Exception;
		class IOException;

		virtual ~BaseResource() = default;
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
}; // namespace job

#endif // JOB_HPP