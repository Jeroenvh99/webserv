#ifndef JOB_HPP
# define JOB_HPP

# include "webserv.hpp"
# include "Environment.hpp"
# include "http/Status.hpp"
# include "route.hpp"

# include <optional>

namespace job {
	enum class Status {
		success,
		failure,
		aborted,
		pending,
	}; // enum class Status

	struct Job {
		Job(Client const&, Server const&);

		bool	is_cgi() const noexcept;

		http::Request const&	request;
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
		ErrorJob(http::Status, Server const&);
		ErrorJob(http::Status, Job const&);

		http::Status const	status;
		stdfs::path const&	file;
	}; // struct ErrorJob
}; // namespace job

#endif // JOB_HPP