#ifndef ROUTE_HPP
# define ROUTE_HPP

# include "http.hpp"
# include "http/Status.hpp"

# include <initializer_list>
# include <type_traits>

class Route {
public:
	Route(std::string const&, std::string const& = directory_listing,
		std::initializer_list<http::Method> = http::any_method);
	Route(Route const&, std::string const&);

	static constexpr char const*	directory_listing = "";
	
	std::string const&	to() const noexcept;
	std::string const&	default_file() const noexcept;
	bool				allows_method(http::Method) const noexcept;

private:
	using Bitmask = std::underlying_type<http::Method>::type;

	std::string	_to;
	std::string	_dfl;
	Bitmask		_methods;
}; // class Route

/*
class Route {
public:
	Route(std::file
private:
	using MethodBitmask = std::underlying_type<http::Method>::type;
	std::vector<Route>			_subroutes;
	std::string					_from;
	std::filesystem::path		_to;
	MethodBitmask				_allowed_methods;
	std::string					_default_file;
	bool						_directory_listing;
	std::vector<std::string>	_cgi;
}; // class Route
*/

#endif // ROUTE_HPP
