#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "network/Acceptor.hpp"
# include "network/Handle.hpp"
# include "network/Poller.hpp"
# include "logging/logging.hpp"
# include "http/Status.hpp"
# include "Client.hpp"
# include "route.hpp"
# include "Config.hpp"

# include <filesystem>
# include <string>
# include <unordered_map>
# include <vector>

class VirtualServer {
	public:
		struct Redirection {
			URI		from;
			URI		to;
			bool	permanent;
		};
		using ErrorPageMap = std::unordered_map<http::Status, std::filesystem::path>;
		VirtualServer(Config::Server config);
		std::string const&						name() const noexcept;
		route::Route const&						route() const noexcept;
		route::Location							locate(std::filesystem::path const&) const;
		route::Location							
			locate(URI const&) const;
		stdfs::path const&						locate_errpage(http::Status) const noexcept;
		std::vector<VirtualServer::Redirection>	getRedirections() const;
		void									add_httpredirect(std::string from, std::string to, bool permanent);
		static stdfs::path const				no_errpage;
	private:
		std::string					_name;
		route::Route				_route;
		ErrorPageMap				_error_pages;
		std::vector<Redirection>	_redirections;
};

class Server {
public:
	using Acceptor = network::Acceptor<network::Domain::ipv4>;
	using SharedHandle = network::SharedHandle;

	Server() = delete;
	~Server() = default;
	Server(Server const&) = delete;
	Server(Server&&) = default;
	Server(Config::Server, int, std::ostream&, std::ostream&);
	// Server(Config&&);
	Server&	operator=(Server const&) = delete;
	Server&	operator=(Server&&);

	in_port_t							port() const noexcept;

	Acceptor&							acceptor() noexcept;
	Acceptor const&						acceptor() const noexcept;

	void								addVirtualServer(Config::Server config);
	VirtualServer const&				searchVirtualServer(std::string name);

	void	process();

private:
	using LogLevel = logging::ErrorLogger::Level;
	using ClientIt = ClientMap::iterator;

	using IOStatus = webserv::GenericStatus;

	void		_accept();
	void		_process_core(network::Poller::Event const&, ClientIt);
	IOStatus	_process_read(Client&);
	IOStatus	_process_write(Client&);
	void		_process_graveyard(network::Poller::Event const&, ClientIt);
	void		_to_graveyard(ClientIt);
	void		_drop(ClientIt);

	IOStatus	_parse_request(Client&);
	IOStatus	_parse_response(Client&);
	IOStatus	_fetch(Client&, webserv::Buffer&);
	IOStatus	_enchunk_and_send(Client&);
	IOStatus	_fetch_and_send(Client&);
	IOStatus	_deliver(Client&);
	IOStatus	_dechunk(Client&);
	IOStatus	_recv(Client&, webserv::Buffer&);
	IOStatus	_send(Client&);
	IOStatus	_send(Client&, webserv::Buffer const&);

	SharedHandle							_acceptor;
	ClientMap								_clients;
	ClientMap								_graveyard;
	std::map<std::string, VirtualServer>	_possibleservers;
	logging::AccessLogger	_alog;
	logging::ErrorLogger	_elog;
}; // class Server

#endif // SERVER_HPP
