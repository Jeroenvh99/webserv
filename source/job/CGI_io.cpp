#include "job/CGI.hpp"
#include "Poller.hpp"

using job::CGI;
using EventType = webserv::Poller::EventType;

// Note: if I/O fails due to the CGI socket not appearing in the poller list,
// an exception will still be thrown

CGI::Socket const&
CGI::socket() const {
	if (_socket == nullptr)
		throw (Exception("uninitialized socket"));
	return (static_cast<CGI::Socket const&>(*_socket));
}

size_t
CGI::read(webserv::Buffer& buf) {
	flush();
	try {
		if (!g_poller.event(_socket).happened(EventType::read))
			throw (IOException("socket unavailable for reading"));
		return (socket().read(buf));
	} catch (std::out_of_range&) {
		throw (IOException("socket unavailable"));
	}
}

size_t
CGI::write(webserv::Buffer const& buf) {
	_obuf += std::string(buf);
	return (buf.len());
}

void
CGI::flush() {
	if (_obuf.length() == 0)
		return;
	try {
		if (!g_poller.event(_socket).happened(EventType::write))
			throw (IOException("socket unavailable for writing"));
		socket().write(_obuf);
		_obuf.clear();
	} catch (std::out_of_range&) {} // socket was not included in poller list
}