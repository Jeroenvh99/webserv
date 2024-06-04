# Response
```cpp
class http::Response: public http::Message;
```
Class representing a HTTP response message.
## Methods
### Constructors
```cpp
Response::Response(std::string const& body, http::Status status); // 1
Response::Response(std::string&& body, http::Status status);      // 2
Response::Response(http::Status status);                          // 3
```
1. Initialize the response body to a copy of `body` (defaults to empty) and the status to `status` (defaults to `http::Status::ok`).
2. As 1, but moves `body` into the response.
3. Initialize the response status to `status` and the body to a minimal default HTML document.
### Accessors
#### body
```cpp
std::string const& Response::body() const noexcept;
```
Get the response body.
#### status
```cpp
http::Status Response::status() const noexcept;
```
Get the response's HTTP status.
#### version
```cpp
http::Version Response::version() const noexcept;
```
Get the response's HTTP version. This will always be equal to `http::Version(1, 1)`.
### Modifiers
#### clear
```cpp
void Response::clear() noexcept;
```
Restore this response to its default-constructed state.
## Non-member functions
### I/O
#### operator <<
```cpp
std::ostream& http::operator<<(std::ostream& os, Response const& resp);
```
Insert a HTTP 1.1-conforming representation of `resp` into `os.`
##### Return values
- `os`