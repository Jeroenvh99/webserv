#ifndef SERVER_HPP
# define SERVER_HPP

# include "network.hpp"
# include "network/Buffer.hpp"
# include "source/config/ConfigParser.hpp" // why is this here?
# include "http/StatusCode.hpp"

# include <map>
# include <vector>

class Server {
public:
	using Acceptor = network::Acceptor<network::Domain::ipv4>;
	using Buffer = network::Buffer<512>;
	using Client = network::StreamSocket<network::Domain::ipv4>;
	using Clients = std::vector<Client>;
	using network::SharedHandle;
	using network::Poller;
	using ErrorPageMap = std::map<http::StatusCode, std::string>;
	using RouteMap = std::map<std::string, Route>;

	Server() = delete;
	Server~() = default;
	Server(Server const&) = delete;
	Server(Server&&) = delete;
	Server(Config const&);
	Server(Config&&);
	Server&	operator=(Server const&) = delete;
	Server&	operator=(Server&&) = delete;

	void	loop();

private:
	Poller			_poller;
	SharedHandle	_acceptor;
	Clients			_clients;
	RouteMap		_routes;
	ErrorPageMap	_error_pages;
	Buffer			_buffer;
}; // class Server

#endif // SERVER_HPP
