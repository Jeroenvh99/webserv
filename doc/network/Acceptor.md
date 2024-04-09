# Acceptor
```cpp
template<network::Domain DOMAIN>
class network::Acceptor: public network::StreamSocket<DOMAIN>;
```
Class representing a socket that can listen for incoming connections.
## Member types
- `Address`
Inherited from `StreamSocket`.
- `OptionList`
Inherited from `StreamSocket`.
## Methods
### Constructors
```cpp
Acceptor<DOMAIN>::Acceptor(OptionList opts, char const* prot);                                   // 1
Acceptor<DOMAIN>::Acceptor(Address const& addr, OptionList opts, char const* prot);              // 2
Acceptor<DOMAIN>::Acceptor(int backlog, Address const& addr, OptionList opts, char const* prot); // 3
```
1. Opens a socket.
2. Opens a socket and binds it to `addr`.
3. Opens a socket, binds it to addr and starts listening with a backlog size of `backlog`.
### Utility
#### listen
```cpp
void
Acceptor<DOMAIN>::listen(int backlog) const;
```
Listen for incoming connections with a maximum queue size of `backlog`.
##### Exceptions
- `SocketException` if `listen()` fails.
#### accept
```cpp
StreamSocket<DOMAIN>
Acceptor<DOMAIN>::accept() const;                                    // 1
StreamSocket<DOMAIN>
Acceptor<DOMAIN>::accept(StreamSocket<DOMAIN>::Address& addr) const; // 2
```
1. Accept an incoming connection request and create a new stream socket for it. Unless `SocketOption::nonblock` was set for this acceptor, the calling thread is blocked until a connection request is received (an exception is thrown otherwise).
2. As 1, but also stores the address of the new socket in `addr`, potentially saving a call to `StreamSocket<DOMAIN>::address()` later.
##### Exceptions
- `SocketException` if `accept()` fails.