#include "EventList.hpp"

using webserv::EventList;

// Modifiers

void
EventList::clean() noexcept {
	for (auto it = this->begin(), it != this->end(), ++it)
		if (it->first.use_count() == 1)
			erase(it);
}