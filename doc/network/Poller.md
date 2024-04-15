# POLLER
```cpp
class network::Poller: public Handle;
```
Class representing a collection of sockets that can be _polled_ for the availability of I/O operations.
## Member types
- `class Event`: A C++ representation of an event that might occur on a particular file descriptor. See more at the [Poller::Event] documentation.
- `enum class EventType`: An enumeration representing the type of event to occur. Possible values:
	- `read`: Data can be read from this file.
	- `write`: Data can be written to this file.
	- `closed`: The file was closed unexpectedly.
	- `exception`: An exceptional condition has occurred with this file.
	- `error`: An error occurred with this file.
	- `hangup`: The peer connection has been closed.
- `enum class Mode`: An enumeration representing the poller modality for a particular file descriptor. Possible values:
	- `edge_triggered`
	- `one_shot`
	- `wake_up`
	- `exclusive`
- `EventTypes` and `Modes`: Aggregate types containing an arbitrary number of `EventType` or `Mode` values.
- `Events`. An aggregate type containing a collection of Events that may be detected after `wait()`ing for them.
## Methods
### Basic operations
```cpp
Poller();                              // 1
~Poller() = default;                   // 2
Poller(Poller const& that) = delete;   // 3
Poller(Poller&& that);                 // 4
Poller& operator=(Poller const& that); // 5
Poller&	operator=(Poller&& that);      // 6
```
1. Default constructor: creates a new poller instance (using the `epoll` API).
2. Destructor: default implementation.
3. Copy constructor: explicitly deleted.
4. Move constructor.
5. Copy assigner: explicitly deleted.
6. Move assigner.
### Accessors
#### size
```cpp
size_t	Poller::size() const noexcept;
```
- Return the number of handles that are currently on this poller's interest list.
### Utility
#### add
```cpp
template<typename T>
SharedHandle	Poller::add(T&& handle, EventTypes events, Modes modes);
```
- `handle` should be an instance of class `network::Handle` or of a derived class.
- Add `handle` to the interest list, checking for events of `events` type in `modes` modes. This involves a call to `epoll_ctl()` and also converts `handle` into a shared handle, which is from henceforth (also) owned by this poller.
#### wait
```cpp
template<size_t MAX_EVENTS>
Poller::Events	Poller::wait(unsigned int timeout) const;
```
- Return the events detected on this poller's interest list within `timeout` milliseconds, or up to `MAX_EVENTS` events.
#### modify
```cpp
void	Poller::modify(SharedHandle const& handle, EventTypes events, Modes modes);
```
- Modify the poller settings for `handle` to those specified by `events` and `modes`.
#### remove
```cpp
void	Poller::remove(SharedHandle const& handle);
```
- Remove `handle` from the poller's interest list. After calling this function, this poller will no longer be a shared owner of `handle`.