# Buffer
```cpp
template<size_t SIZE>
class network::Buffer;
```
Class that stores a byte buffer with a capacity of `SIZE`, to be used with a socket's `read()` and `write()` methods.
## Member types
- `iterator` Iterator.
- `const_iterator` Constant iterator.
## Methods
### Accessors
#### len
```cpp
size_t	len() const noexcept;
```
Return the number of readable bytes in this buffer.
### Modifiers
#### empty
```cpp
void	empty() noexcept;
```
Put the buffer in an empty state: that is, to have zero readable bytes.
### Iteration
#### begin, cbegin
```cpp
Buffer::iterator		Buffer::begin() noexcept;        // 1
Buffer::const_iterator	Buffer::begin() const noexcept;  // 2
Buffer::const_iterator	Buffer::cbegin() const noexcept; // 3
```
1. Return an iterator to the first byte.
2. Return a constant iterator to the first byte.
3. Return a constant iterator to the first byte.

#### end, cend
```cpp
Buffer::iterator		Buffer::end() noexcept;        // 1
Buffer::const_iterator	Buffer::end() const noexcept;  // 2
Buffer::const_iterator	Buffer::cend() const noexcept; // 3
```
1. Return an iterator to past the last readable byte.
2. Return a constant iterator to past the last readable byte.
3. Return a constant iterator to past the last readable byte.

## Non-member functions
```cpp
std::istream&	operator>>(std::istream& is, Buffer<SIZE>& obj); // 1
std::ostream&	operator<<(std::ostream& os, Buffer<SIZE> const& obj); // 2
```
1. Extract up to `SIZE` bytes from `is` and store them in `obj`.
2. Insert all readable bytes in `obj` into `os`.