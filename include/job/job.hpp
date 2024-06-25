#ifndef JOB_HPP
# define JOB_HPP

# include "webserv.hpp"
# include "http/Status.hpp"
# include "route.hpp"

# include <optional>

namespace job {
	using StatusOption = std::optional<http::Status>;
	using ExitOption = std::optional<int>;

	struct Job {
		Job(Client const&, Server const&);

		bool	is_cgi() const noexcept;

		http::Request const&	request;
		route::Location const	location;
		Environment const&		environment;
	}; // struct Job

	struct ErrorJob {
		ErrorJob(http::Status, Server const&);

		http::Status const	status;
		stdfs::path const&	file;
	}; // struct ErrorJob
}; // namespace job

#endif // JOB_HPP