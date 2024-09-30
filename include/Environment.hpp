#ifndef ENVIRONMENT_HPP
# define ENVIRONMENT_HPP

# include "webserv.hpp"
# include "http/Request.hpp"
# include "route.hpp"

# include <array>
# include <string>
# include <vector>

class Environment {
public:
	using Container = std::vector<std::string>;

	Environment(Server const&, VirtualServer const&, Client const&, route::Location const&);
	~Environment();

	void	build();

	Container const&	vec() const noexcept;
	char const* const*	cenv() const noexcept;
	char**				cenv() noexcept;

	static std::string	env_string(std::string const&, std::string const&);
	static std::string	env_string(http::Header const&);

	static constexpr size_t			dfl_size = 17;
	// DB: See RFC 3875, section 4.1 (CGI 1.1; Request Meta-Variables)
	static constexpr size_t			static_size = 2;
	using StaticArray = std::array<char const*, static_size>;
	static constexpr StaticArray	static_env = {
		"SERVER_SOFTWARE=Webserv/1.0",
		"GATEWAY_INTERFACE=CGI/1.1"};

private:
	friend int main(int, char**, char**);

	void	append(Client const&);
	void	append(http::Request const&);
	void	append(http::Header const&);
	void	append(std::string const&, std::string const&);
	void	_build_cenv();

	Server const&			_server;
	VirtualServer const&	_vserver;
	Client const&			_client;
	route::Location const&	_location;

	Container	_ctr;
	char**		_cenv;

	static char**	_parent_env;
	static size_t	_parent_env_size;
}; // class Environment

#endif // ENVIRONMENT_HPP
