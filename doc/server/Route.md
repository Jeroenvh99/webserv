# Route
```cpp
class route::Route: public Config;
```
A class representing a mapping of URI paths to various options that might be configured for those URI's on a particular server.

## Associated types
- `Path` Inherited from `Config`.

## Methods
### Constructors
```cpp
Route(Path const& path); // 1
```
1. Create a new route tree which automatically extends to follow `path`.

### Modifiers
#### extend
```cpp
Route&	Route::extend(Path const& path);
```
Extend a route by appending new nodes for every segment of `path` that is not yet contained in this route. If the route already covers the entirety of `path`, the behaviour is identical to `Route::seek`.
##### Return value
A reference to the lowermost added route node (i.e. the node corresponding to the final segment of `path`).

#### seek
```cpp
Route&	Route::seek(Path const& path);
```
Find the route node described by `path`.
##### Return value
A reference to the queried route node.
##### Exceptions
- `std::invalid_argument` if the route does not extend to cover `path`.

#### follow
```cpp
Location	Route::follow(Path const& path) const;
```
Retrieve the configuration that applies to `path` according to this route.
##### Exceptions
- `std::invalid_argument` if the route does not cover `path` - that is, if the route and `path` do not have the same path root.