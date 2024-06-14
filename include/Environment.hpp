#ifndef ENVIRONMENT_HPP
# define ENVIRONMENT_HPP

# include <array>
# include <initializer_list>
# include <string>
# include <vector>

# include "http/Message.hpp"
# include "http/Request.hpp"

class Client;

class Environment {
public:
	Environment(std::initializer_list<std::string>);

	void	append(Client const&);
	void	append(http::Request const&);
	void	append(std::string const&, std::string const&);
	void	append(http::Header const&);
	char**	make_cenv();

	static std::string	env_string(std::string const&, std::string const&);
	static std::string	env_string(http::Header const&);

	static constexpr size_t							dfl_size = 17;
	// DB: See RFC 3875, section 4.1 (CGI 1.1; Request Meta-Variables)
	static constexpr size_t							base_size = 2;
	static std::array<std::string, base_size> const	base_vars;

private:
	friend int main(int, char**, char**);

	static char**	_cenv;
	static size_t	_cenv_size;

	std::vector<std::string>	_local;
}; // class Environment

#endif // ENVIRONMENT_HPP
