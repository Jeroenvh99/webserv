#ifndef ENVIRONMENT_HPP
# define ENVIRONMENT_HPP

# include "webserv.hpp"
# include "http/Message.hpp"
# include "http/Request.hpp"
# include "route.hpp"

# include <array>
# include <initializer_list>
# include <string>
# include <vector>

class Environment {
public:
	using Container = std::vector<std::string>;
	// DB: This effectively allows duplicate variables. We could switch to a map to prevent that.

	Environment(std::initializer_list<std::string>);
	Environment(Environment const&, route::Location const&, Client const&);

	Container const&	ctr() const noexcept;

	void	append(Client const&);
	void	append(http::Request const&);
	void	append(http::Header const&);
	void	append(std::string const&, std::string const&);

	char**	make_cenv();

	static std::string	env_string(std::string const&, std::string const&);
	static std::string	env_string(http::Header const&);

	static constexpr size_t			dfl_size = 17;
	// DB: See RFC 3875, section 4.1 (CGI 1.1; Request Meta-Variables)
	static constexpr size_t			static_size = 2;
	using StaticContainer = std::array<std::string, static_size>;
	static StaticContainer const	static_env;

private:
	friend int main(int, char**, char**);

	static char**	_cenv;
	static size_t	_cenv_size;

	Container	_ctr;
}; // class Environment

#endif // ENVIRONMENT_HPP
