#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"
# include "network/Handle.hpp"

class Client {
public:
	using Socket = network::IPv4StreamSocket;
	Client(Socket&&);

private:
	Socket	_socket;
}; // class Client

#endif // CLIENT_HPP