# Event
```cpp
class Poller::Event
```
Class representing an I/O event that can occur on a Handle and be detected by a Poller.
## Methods
### Accessors
#### happened
```cpp
bool	Event::happened(EventType event) const noexcept;
```
Return whether an event of type `event` has occurred.
#### handle
```cpp
SharedHandle	Event::handle() const noexcept;
```
Return the handle on which the event(s) occurred.
##### Notes
- This methods returns a shared handle, meaning that ownership will be shared with the calling scope. This allows, for example, the easy removal of this handle from the poller that generated this event in case a hangup event or a zero-read was detected.