#ifndef POLLER_HPP
# define POLLER_HPP

# include "network/Poller.hpp"

namespace webserv {
	class Poller: public network::Poller {
	public:
		static constexpr size_t			max_events = 1024;
		static constexpr unsigned int	timeout = 1000;

		Events::iterator	begin() noexcept;
		Events::iterator	end() noexcept;

		void	clear_events() noexcept;
		void	wait();

	private:
		Events	_events;
	}; // class Poller
}; // namespace webserv

extern webserv::Poller	g_poller;

#endif // POLLER_HPP