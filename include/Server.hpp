#ifndef SERVER_HPP
# define SERVER_HPP

# include "network/Acceptor.hpp"
# include "network/Address_IPv4.hpp"
# include "network/Buffer.hpp"
# include "network/Handle.hpp"
# include "network/StreamSocket.hpp"
# include "network/Poller.hpp"
# include "logging.hpp"
# include "http/StatusCode.hpp"
# include "Buffer.hpp"
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

	void	_process_events(Poller::Events const&);
	void	_client_add();
	void	_client_handle(ClientMap::iterator, Poller::Event const&);
	void	_client_handle_graveyard(ClientMap::iterator, Poller::Event const&);
	void	_client_to_graveyard(ClientMap::iterator);
	void	_client_drop(ClientMap::iterator);
	void	_process_buffer(Client&);

	Poller					_poller;
	SharedHandle			_acceptor;
	ClientMap				_clients;
	ClientMap				_graveyard;
	RouteMap				_routes;
	ErrorPageMap			_error_pages;
	Buffer					_buffer;
	logging::AccessLogger	_alog;
	logging::ErrorLogger	_elog;
}; // class Server

#endif // SERVER_HPP
