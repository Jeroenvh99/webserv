#include "Environment.hpp"

// Static variables

char**	Environment::_parent_env;
size_t	Environment::_parent_env_size;

// Basic operations

Environment::Environment(Server const& server, VirtualServer const& vserver, Client const& client, route::Location const& location):
	_server(server),
	_vserver(vserver),
	_client(client),
	_location(location),
	_cenv(nullptr) {}

Environment::~Environment() {
	delete[] _cenv;
}
