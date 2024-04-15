# StreamSocket
```cpp
template<network::Domain DOMAIN>
class network::StreamSocket: public network::Socket<DOMAIN, Type::stream>;
```
Partial specialization of the base Socket.
This is the socket type that will be created by an Acceptor's `accept()` method.