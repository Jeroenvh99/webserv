#ifndef NETWORK_HPP
# define NETWORK_HPP

# include <cstddef>
# include <stdexcept>
# include <string>

extern "C" {
# include <sys/socket.h>
# include <sys/types.h>
}

namespace network {

enum class Domain {
	ipv4 = AF_INET,
	ipv6 = AF_INET6,
	local = AF_LOCAL,
}; // enum class Domain

enum class Type {
	stream = SOCK_STREAM,
}; // enum class Type

class Handle;
	
template<Domain, Type>
class Socket;

template<Domain>
class Address;

template<Domain>
class StreamSocket;

template<Domain>
class Acceptor;

template<size_t>
class Buffer;

class Poller;

class Exception: public std::exception {
public:
	Exception(std::string const&);

	char const*	what() const noexcept;
private:
	std::string	_msg;
}; // class Exception

}; // namespace network

#endif // NETWORK_HPP
