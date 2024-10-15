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
CGI::read(webserv::Buffer& buf) const {
	try {
		if (!g_poller.event(_socket).happened(EventType::read))
			throw (IOException("socket unavailable for reading"));
		return (socket().read(buf));
	} catch (std::out_of_range&) {
		throw (IOException("socket unavailable"));
	}
}

size_t
CGI::write(webserv::Buffer const& buf) const {
	try {
		if (!g_poller.event(_socket).happened(EventType::write))
			throw (IOException("socket unavailable for writing"));
		if (_ibuf.length() > 0) {
			_ibuf += std::string(buf);
			socket().write(_ibuf);
			_ibuf = "";
			return (buf.len());
		}
		return (socket().write(buf));
	} catch (std::out_of_range&) {
		_ibuf += std::string(buf);
		return (buf.len());
	}
}