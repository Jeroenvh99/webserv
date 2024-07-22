#include "job/CGI.hpp"
#include "Poller.hpp"

using job::CGI;
using EventType = webserv::Poller::EventType;

std::optional<size_t>
CGI::read(webserv::Buffer& buf) const {
	auto	eventit = std::find(g_poller.begin(), g_poller.end(), _socket);

	if (eventit == g_poller.end() || !eventit->happened(EventType::read))
		return (std::nullopt);
	return (_socket->read(buf));
}

std::optional<size_t>
CGI::write(webserv::Buffer const& buf) const {
	auto	eventit = std::find(g_poller.begin(), g_poller.end(), _socket);

	if (eventit == g_poller.end() || !eventit->happened(EventType::write))
		return (std::nullopt);
	return (_socket->write(buf));
}