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
	using Clients = std::vector<Client>;
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
	void	_process_events(Poller::Events const&);
	void	_add_client();
	void	_handle_client(Poller::Event const&, Clients::iterator);
	void	_drop_client(Clients::iterator);
	void	_process_buffer(Client&);

	Poller					_poller;
	SharedHandle			_acceptor;
	Clients					_clients;
	RouteMap				_routes;
	ErrorPageMap			_error_pages;
	Buffer					_buffer;
	logging::AccessLogger	_alog;
	logging::ErrorLogger	_elog;
}; // class Server

#endif // SERVER_HPP
