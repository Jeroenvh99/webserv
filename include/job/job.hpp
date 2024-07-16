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
		timeout,
	}; // enum class Status

	struct Job {
		Job(Client const&, Server const&);

		bool	is_cgi() const noexcept;

		http::Request const&	request;
		Server const&			server;
		route::Location const	location;
		Environment				environment;
	}; // struct Job

	struct ErrorJob {
		ErrorJob(http::Status, Server const&);
		ErrorJob(http::Status, Job const&);

		http::Status const	status;
		stdfs::path const&	file;
	}; // struct ErrorJob
}; // namespace job

#endif // JOB_HPP