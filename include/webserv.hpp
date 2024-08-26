#ifndef WEBSERV_HPP
# define WEBSERV_HPP

class Server;
class VirtualServer;
class Client;
class Environment;
class URI;

namespace webserv {
	enum class GenericStatus {
		success,
		failure,
		pending,
	}; // enum class GenericStatus
}; // namespace webserv

#endif // WEBSERV_HPP