# Handle
```cpp
class network::Handle
```
A class that serves as a wrapper around the humble Unix file descriptor, mainly managing its lifetime.

## Associated types
- `SharedHandle`
A reference to a Handle of which the ownership is shared amongst several scopes. This type is mainly used in the context of the `Poller`.

## Member types
- `Exception`	Exception that can be thrown when a lower-level operation on the file descriptor fails (e.g. where C-style `open()` would return -1 on failure, the Handle will throw a `Handle::Exception`).
- `Raw`	The type of the underlying file descriptor: equates `int`.

## Member constants
- `static constexpr Raw	_invalid_handle = -1;`

## Methods
### Constructors
```cpp
Handle();                                  // 1
virtual ~Handle();                         // 2
Handle(Raw fd);                            // 3
Handle(Handle const& that);                // 4
Handle(Handle&& that) noexcept;            // 5
Handle&	operator=(Handle const& that);	   // 6
Handle&	operator=(Handle&& that) noexcept; // 7
```
1. Default constructor: initialize the underlying file descriptor to `_invalid_handle`.
2. Destructor: `close()` the underlying file descriptor.
3. Initialize the underlying file descriptor to `fd`.
4. Copy constructor: initialize the underlying file descriptor to a `dup()`licate of `that`.
5. Move constructor: initialize the underlying file descriptor to `that`; its file descriptor is set to `_invalid_handle`.
6. Copy assigner: set the underlying file descriptor to a `dup()`licate of `that`.
7. Move assigner: set  the underlying file descriptor to `that`; its file descriptor is set to `_invalid_handle`.

### Other methods
All other methods are `protected`.
#### raw
```cpp
Handle::Raw	Handle::raw() const noexcept;
```
Return the raw file descriptor.
#### close
```cpp
void	Handle::close();
```
Release the underlying file descriptor and close it.
##### Exceptions
- `Handle::Exception` on failure of `close()`.

#### release
```cpp
Handle::Raw	Handle::release() noexcept;
```
Release the underlying file descriptor and return it to the caller.

