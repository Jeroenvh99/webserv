#include "job/job.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "route.hpp"

using job::Job;
using job::ErrorJob;

/* Job */

// Basic operations

Job::Job(Client const& client, Server const& serv, VirtualServer const& vserv):
	request(client.request()),
	vserver(vserv),
	server(serv),
	location(vserver.locate(client.request().uri())),
	environment(server, vserver, client, location) {}

job::RedirectionJob::RedirectionJob(job::Job const& todo, int redirindex):
	destination(todo.vserver.redirections()[redirindex].to),
	permanent(todo.vserver.redirections()[redirindex].permanent) {}

// Methods

int
job::is_httpredirect(job::Job const& job) {
	auto const& redirs = job.vserver.redirections();
	for (size_t i = 0; i < redirs.size(); i++) {
		if (job.location.from() == redirs[i].from.path()) {
			return i;
		}
	}
	return -1;
}

bool
Job::is_cgi() const noexcept {
	return (location.is_cgi());
}

/* ErrorJob */

// Basic operations

ErrorJob::ErrorJob(http::Status _status, VirtualServer const& vserver):
	status(_status), file(vserver.locate_errpage(_status)) {}

ErrorJob::ErrorJob(http::Status _status, Job const& job):
	ErrorJob(_status, job.vserver) {}