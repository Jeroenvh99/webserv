#ifndef EVENT_LIST_HPP
# define EVENT_LIST_HPP

# include "network/Poller.hpp"

namespace webserv {
	class EventList: public std::unordered_map<network::SharedHandle, network::Poller::Event> {
	public:
		using super = std::unordered_map<network::SharedHandle, network::Poller::Event>;
		
		using super::insert;
		void	insert(network::SharedHandle const&);
		size_t	clean() noexcept;
	};
}; // namespace webserv

extern EventList	g_cgi_sockets;

#endif // EVENT_LIST_HPP