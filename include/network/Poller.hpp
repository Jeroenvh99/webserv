#ifndef NETWORK_POLLER_HPP
# define NETWORK_POLLER_HPP

# include "network.hpp"
# include "network/Handle.hpp"
# include "network/Handle.hpp"

# include <cstddef>
# include <cstdint>
# include <initializer_list>
# include <unordered_map>
# include <unordered_set>

extern "C" {
# include <sys/epoll.h>
}

namespace network {
	class Poller: public Handle {
	public:
		class Event;
		class Exception;
		enum class EventType: uint32_t;
		enum class Mode: uint32_t;
		using EventTypes = std::initializer_list<EventType>;
		using Modes = std::initializer_list<Mode>;
		using Events = std::unordered_map<SharedHandle, Event>;

		Poller();
		~Poller() = default;
		Poller(Poller const&) = delete;
		Poller(Poller&&);
		Poller&	operator=(Poller const&) = delete;
		Poller&	operator=(Poller&&);

		template<typename T>
		SharedHandle	add(T&&, EventTypes, Modes);
		void			add_shared(SharedHandle const&, EventTypes, Modes);
		template<size_t MAX_EVENTS>
		Events			wait(unsigned int) const;
		void			modify(SharedHandle const&, EventTypes, Modes);
		void			remove(SharedHandle const&);
		size_t			size() const noexcept;

	private:
		using Handles = std::unordered_set<SharedHandle>;

		Handles	_handles;
	}; // class Poller

	class Poller::Event {
	public:
		bool	expire(EventType) noexcept;
		bool	happened(EventType) const noexcept;

	private:
		friend class Poller;
		using Raw = epoll_event;

		Event(Raw);

		Raw	_raw;
	}; // class Event

	class Poller::Exception: public network::Exception {
	public:
		Exception(char const*);
	}; // class Poller::Exception

	enum class Poller::Mode: uint32_t {
		edge_triggered = EPOLLET,
		one_shot = EPOLLONESHOT,
		wake_up = EPOLLWAKEUP,
		exclusive = EPOLLEXCLUSIVE,
	}; // enum class Poller::Mode

	enum class Poller::EventType: uint32_t {
		read = EPOLLIN,
		write = EPOLLOUT,
		closed = EPOLLRDHUP,
		exception = EPOLLPRI,
		error = EPOLLERR,
		hangup = EPOLLHUP,
	}; // enum class Poller::EventType
}; // namespace network

# include "network/Poller_method.tpp"

#endif // NETWORK_POLLER_HPP
