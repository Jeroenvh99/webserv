#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "network/Acceptor.hpp"
# include "network/Handle.hpp"
# include "network/Poller.hpp"
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
		using Redirections = std::vector<Redirection>;

		VirtualServer(Config::Server config);
		std::string const&			name() const noexcept;
		int const&					port() const noexcept;
		route::Route const&			route() const noexcept;
		route::Location				locate(std::filesystem::path const&) const;
		route::Location				locate(URI const&) const;
		stdfs::path const&			locate_errpage(http::Status) const noexcept;
		size_t						max_body_size() const noexcept;
		Redirections const&			redirections() const noexcept;
		void						add_httpredirect(std::string from, std::string to, bool permanent);
		static stdfs::path const	no_errpage;
	private:
		std::string					_name;
		route::Route				_route;
		int							_maxbodysize;
		ErrorPageMap				_error_pages;
		std::vector<Redirection>	_redirections;
		int							_port;
};

class Server {
public:
	using Acceptor = network::Acceptor<network::Domain::ipv4>;
	using SharedHandle = network::SharedHandle;

	Server() = delete;
	~Server() = default;
	Server(Server const&) = delete;
	Server(Server&&) = default;
	Server(Config::Server, int);
	// Server(Config&&);
	Server&	operator=(Server const&) = delete;
	Server&	operator=(Server&&);

	in_port_t				port() const noexcept;

	Acceptor&				acceptor() noexcept;
	Acceptor const&			acceptor() const noexcept;

	void					virtual_server_add(Config::Server config);
	VirtualServer const&	virtual_server(std::string const& name);
	VirtualServer const&	virtual_server(Client const&);

	void	process();

private:
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
	IOStatus	_recv_and_deliver(Client&);
	IOStatus	_dechunk_and_deliver(Client&);
	IOStatus	_deliver(Client&, webserv::Buffer const&);
	IOStatus	_recv(Client&, webserv::Buffer&);
	IOStatus	_send(Client&);
	IOStatus	_send(Client&, webserv::Buffer const&);

	SharedHandle				_acceptor;
	ClientMap					_clients;
	ClientMap					_graveyard;
	std::vector<VirtualServer>	_possibleservers;
}; // class Server

#endif // SERVER_HPP
