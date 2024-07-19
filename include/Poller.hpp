#ifndef POLLER_HPP
# define POLLER_HPP

# include "network/Poller.hpp"

namespace webserv {
	class Poller {
		
	}

	class EventList: public std::unordered_map<network::SharedHandle, network::Poller::Event> {
	public:
		using super = std::unordered_map<network::SharedHandle, network::Poller::Event>;
		
		using super::insert;
		void	insert(network::SharedHandle const&);
		size_t	clean() noexcept;
	};
}; // namespace webserv

extern Poller		g_io;
extern EventList	g_events;

#endif // POLLER_HPP