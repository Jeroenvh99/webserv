#ifndef ROUTE_HPP
# define ROUTE_HPP

# include "http/Request.hpp"
# include "http/StatusCode.hpp"

class Route {
public:
	Route(std::string const&, std::string const&, http::RequestMethod);

	std::string const&	to() const noexcept;
	std::string const&	default_file() const noexcept;
	bool				allows_method(http::RequestMethod) const noexcept;
private:
	std::string			_to;
	std::string			_dfl;
	http::RequestMethod	_methods;
}; // class Route

#endif // ROUTE_HPP
