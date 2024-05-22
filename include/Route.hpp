#ifndef ROUTE_HPP
# define ROUTE_HPP

# include "http.hpp"
# include "http/StatusCode.hpp"

# include <initializer_list>
# include <type_traits>

class Route {
public:
	Route(std::string const&, std::string const&,
		std::initializer_list<http::Method>);

	std::string const&	to() const noexcept;
	std::string const&	default_file() const noexcept;
	bool				allows_method(http::Method) const noexcept;
private:
	using Bitmask = std::underlying_type<http::Method>::type;

	std::string	_to;
	std::string	_dfl;
	Bitmask		_methods;
}; // class Route

#endif // ROUTE_HPP
