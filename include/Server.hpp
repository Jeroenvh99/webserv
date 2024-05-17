#ifndef SERVER_HPP
# define SERVER_HPP

# include "network/Acceptor.hpp"
# include "network/Address_IPv4.hpp"
# include "network/Handle.hpp"
# include "network/StreamSocket.hpp"
# include "network/Poller.hpp"
# include "logging.hpp"
# include "http/StatusCode.hpp"
# include "Client.hpp"
# include "Route.hpp"

# include <map>
# include <string>
# include <vector>

class Server {
public:
	using Acceptor = network::Acceptor<network::Domain::ipv4>;
	using ErrorPageMap = std::map<http::StatusCode, std::string>;
	using Poller = network::Poller;
	using RouteMap = std::map<std::string, Route>;
	using SharedHandle = network::SharedHandle;

	Server() = delete;
	~Server() = default;
	Server(Server const&) = delete;
	Server(Server&&);
	Server(in_port_t, std::ostream& = std::cout, std::ostream& = std::cerr); // remove this once the config parser is done
	// Server(Config&&);
	Server&	operator=(Server const&) = delete;
	Server&	operator=(Server&&);

	Acceptor&		acceptor() noexcept;
	Acceptor const&	acceptor() const noexcept;
	void			loop(int);

	static constexpr Poller::EventTypes	poller_events = {
		Poller::EventType::read, Poller::EventType::write
	};
	static constexpr Poller::Modes		poller_mode = {};

private:
	using LogLevel = logging::ErrorLogger::Level;
	using ClientIt = ClientMap::iterator;

	void	_accept();
	void	_process(Poller::Event const&, ClientIt);
	void	_process_graveyard(Poller::Event const&, ClientIt);
	void	_to_graveyard(ClientIt);
	void	_drop(ClientIt);

	bool	_read(Client&);
	bool	_write(Client&);

	Poller					_poller;
	SharedHandle			_acceptor;
	ClientMap				_clients;
	ClientMap				_graveyard;
	RouteMap				_routes;
	ErrorPageMap			_error_pages;
	logging::AccessLogger	_alog;
	logging::ErrorLogger	_elog;
}; // class Server

#endif // SERVER_HPP
