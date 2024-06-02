# Server
```cpp
class Server;
```
A class that encapsulates a server and its configuration settings. The server comprises an acceptor socket and a collection of client connections created from that socket. The server configuration consists mainly of of a request route, a collection of default HTTP status responses and server activity loggers.

## Methods
### Constructor
```cpp

```

### Modifiers
#### process
```cpp
void Server::process(int poller_timeout);
```
Check the availability of I/O operations on all contained sockets and perform those operations if applicable.