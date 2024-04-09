#ifndef POLLER_HPP
# define POLLER_HPP

# include "network/Poller.hpp"

using network::Poller;
using EventType = Poller::EventType;
using network::Event;

constexpr Poller::EventTypes	dfl_events = {EventType::read, EventType::write};
constexpr Poller::Modes			dfl_mode = {};

#endif // POLLER_HPP
