#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "network.hpp"
# include "network/Acceptor.hpp"
# include "network/Poller.hpp"
# include "network/Address_IPv4.hpp"

using network::Poller;
using EventType = Poller::EventType;

using IPv4Acceptor = network::Acceptor<network::Domain::ipv4>;
using IPv4StreamSocket = network::StreamSocket<network::Domain::ipv4>;

constexpr Poller::EventTypes	dfl_events = {EventType::read, EventType::write};
constexpr Poller::Modes			dfl_mode = {};

#endif // WEBSERV_HPP
