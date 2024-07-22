#ifndef POLLER_HPP
# define POLLER_HPP

# include "network/Poller.hpp"

namespace webserv {
	class Poller: public network::Poller {
	public:
		static constexpr size_t			max_events = 1024;
		static constexpr unsigned int	timeout = 1000;

		static constexpr EventTypes	events = {
			EventType::read, EventType::write
		};
		static constexpr Modes		mode = {};

		Events::iterator	begin() noexcept;
		Events::iterator	end() noexcept;

		void			clear_events() noexcept;
		void			wait();

	private:
		Events	_events;
	}; // class Poller
}; // namespace webserv

// probably to do: have the poller keep track of what port server every registered socket belongs to
// have wait() sort incoming events by server
// every server will only have to loop to their sockets + the CGI list, if applicable
// ???
// profit!

extern webserv::Poller	g_poller;

#endif // POLLER_HPP