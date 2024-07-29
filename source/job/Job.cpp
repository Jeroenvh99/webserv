#include "job/job.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "route.hpp"

using job::Job;
using job::ErrorJob;

/* Job */

// Basic operations

Job::Job(Client const& client, Server const& serv):
	request(client.request()),
	server(serv),
	location(server.locate(client.request().uri())),
	environment(server, client, location) {}

job::RedirectionJob::RedirectionJob(job::Job const& todo, int redirindex):
	destination(todo.server.getRedirections()[redirindex].to),
	permanent(todo.server.getRedirections()[redirindex].permanent) {}

// Methods

int
job::is_httpredirect(job::Job const& job) {
	std::vector<Server::Redirection> redirections = job.server.getRedirections();
	for (size_t i = 0; i < redirections.size(); i++) {
		if (job.location.from() == redirections[i].from.path()) {
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

ErrorJob::ErrorJob(http::Status _status, Server const& server):
	status(_status), file(server.locate_errpage(_status)) {}

ErrorJob::ErrorJob(http::Status _status, Job const& job):
	ErrorJob(_status, job.server) {}