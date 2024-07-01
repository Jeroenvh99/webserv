#include "job/job.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "route.hpp"

using job::Job;
using job::ErrorJob;

/* Job */

// Basic operations

Job::Job(Client const& client, Server const& server):
	request(client.request()),
	location(server.locate(client.request().uri())),
	environment(server, client, location) {}

// Methods

bool
Job::is_cgi() const noexcept {
	return (location.is_cgi());
}

/* ErrorJob */

// Basic operations

ErrorJob::ErrorJob(http::Status _status, Server const& server):
	status(_status), file(server.locate_errpage(_status)) {}