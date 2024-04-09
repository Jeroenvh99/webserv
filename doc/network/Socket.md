# Socket
```cpp
template<network::Domain DOMAIN, network::Type TYPE>
class network::Socket: public Handle
```
Class representing a network socket belonging to a particular `DOMAIN` and conforming to a particular `TYPE`.

## Associated types
- `enum class Domain`
The socket's communication domain. Possible values:
	- `ipv4`;
- `enum class Type`
The socket's type. Possible values:
	- `stream`;
- `enum class SocketOption`
Miscellaneous options to be set on the socket's file descriptor. Possible values:
	- `nonblock`;
	- `cloexec`;
- `SocketException`	Exception that can be thrown when a lower-level operation on the socket descriptor fails (e.g. where C-style `socket()` would return -1 on failure, the Socket will throw a `SocketException`).

## Member types
- `Address`
The address type associated with this kind of socket. Equates `network::Address<DOMAIN>`.
- `OptionList`
Aggregate type containing an arbitrary number of `SocketOption`s.

## Methods
### Constructors
```cpp
Socket<DOMAIN, TYPE>::Socket(OptionList opts, char const* prot = ""); // 1
```
1. Initialize the socket descriptor by way of a call to `socket()`. Note that `prot` is passed to `getprotobyname()` to retrieve the protocol number.
##### Exceptions
1. 
	- `Socket::Exception` if `prot` does not denote a valid protocol.
	- `Socket::Exception` on failure of `socket()`.
### Manipulators
#### bind
```cpp
void	Socket<DOMAIN, TYPE>::bind(Address const& addr);
```
Bind this socket to local address `addr`.
##### Exceptions
- `SocketException` on failure of `bind()`.
#### write
```cpp
template<size_t BSIZE>
size_t	Socket<DOMAIN, TYPE>::write(Buffer<BSIZE> const& buf, int flags = 0);
```
Write the contents of `buf` to this socket.
##### Exceptions
- `SocketException` on failure of `send()`.
#### read
```cpp
template<size_t BSIZE>
void	Socket<DOMAIN, TYPE>::read(Buffer<BSIZE>& buf, int flags = 0);
```
Read from this socket into `buf`.
##### Exceptions
- `SocketException` on failure of `recv()`.
### Accessors
#### address
```cpp
Socket<DOMAIN, TYPE>::Address	Socket<DOMAIN, TYPE>::address() const noexcept;
```
Return the address to which this socket has been bound.
##### Notes
- If this socket has not in fact been bound to a local address (i.e. no prior call to `bind`() was made), the returned value is unspecified.