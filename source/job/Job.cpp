#include "Worker.hpp"

/* Job */

// Basic operations

Job::Job(Client const& client, Server const& server):
	request(client.request()),
	location(server.locate(client.request().uri())),
	environment(server.environment(location, client)) {}

// Methods

bool
Job::is_cgi() const noexcept {
	return (loc.is_cgi())
}

/* ErrorJob */

// Basic operations

ErrorJob::ErrorJob(http::Status _status, Server const& server):
	status(status), file(server.locate_error_page(_status)) {}