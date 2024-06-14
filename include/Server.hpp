#ifndef SERVER_HPP
# define SERVER_HPP

# include "network/Acceptor.hpp"
# include "network/Address_IPv4.hpp"
# include "network/Handle.hpp"
# include "network/StreamSocket.hpp"
# include "network/Poller.hpp"
# include "logging.hpp"
# include "http/Status.hpp"
# include "http/Request.hpp"
# include "Environment.hpp"
# include "Client.hpp"
# include "route.hpp"

# include <filesystem>
# include <string>
# include <unordered_map>
# include <vector>

class Server {
public:
	using Acceptor = network::Acceptor<network::Domain::ipv4>;
	using ErrorPageMap = std::unordered_map<http::Status, std::filesystem::path>;
	using Poller = network::Poller;
	using SharedHandle = network::SharedHandle;

	Server() = delete;
	~Server() = default;
	Server(Server const&) = delete;
	Server(Server&&);
	Server(std::string const&, in_port_t, int, std::ostream& = std::cout, std::ostream& = std::cerr); // remove this once the config parser is done
	// Server(Config&&);
	Server&	operator=(Server const&) = delete;
	Server&	operator=(Server&&);

	Acceptor&			acceptor() noexcept;
	Acceptor const&		acceptor() const noexcept;
	void				process(int);
	std::string const&	name() const noexcept;
	in_port_t			port() const noexcept;

	route::Location	locate(std::filesystem::path const&) const;
	
	http::Response	respond(http::Request const&);
	http::Response	respond_error(http::Status);
	http::Status	get(std::string&, http::Request const&);
	http::Status	post(std::string&, http::Request const&);
	http::Status	delete_(std::string&, http::Request const&);

	static constexpr Poller::EventTypes	poller_events = {
		Poller::EventType::read, Poller::EventType::write
	};
	static constexpr Poller::Modes		poller_mode = {};

private:
	using LogLevel = logging::ErrorLogger::Level;
	using ClientIt = ClientMap::iterator;

	void	_accept();
	void	_process_core(Poller::Event const&, ClientIt);
	void	_process_graveyard(Poller::Event const&, ClientIt);
	void	_to_graveyard(ClientIt);
	void	_drop(ClientIt);

	bool	_fetch(Client&);
	bool	_read(Client&);
	bool	_wait(Client&);
	bool	_send(Client&);

	std::string				_name;
	Poller					_poller;
	SharedHandle			_acceptor;
	ClientMap				_clients;
	ClientMap				_graveyard;
	route::Route			_route;
	ErrorPageMap			_error_pages;
	logging::AccessLogger	_alog;
	logging::ErrorLogger	_elog;
	Environment				_env;
}; // class Server

#endif // SERVER_HPP
