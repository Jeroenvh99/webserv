#ifndef SERVER_HPP
# define SERVER_HPP

# include "network.hpp"
# include "Client.hpp"
# include "source/config/ConfigParser.hpp" // why is this here?

# include <vector>

class Server {
public:
	using Acceptor = network::Acceptor<network::Domain::ipv4>;
	using ErrorPage = std::map<
	using network::SharedHandle;
	using network::Poller;

	Server() = delete;
	Server~() = default;
	Server(Server const&) = delete;
	Server(Server&&) = delete;
	Server(Config const&);
	Server(Config&&);
	Server&	operator=(Server const&) = delete;
	Server&	operator=(Server&&) = delete;

private:
	Poller				_poller;
	SharedHandle		_acceptor;
	std::vector<Client>	_clients;	
}; // class Server

#endif // SERVER_HPP
