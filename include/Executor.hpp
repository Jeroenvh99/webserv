#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include "webserv.hpp"
# include "http/Request.hpp"
# include "http/Response.hpp"

# include <fstream>

class Executor {
public:
	enum class State;

	void	execute(http::Request const&);

private:
	std::fstream	fs;
}; // class Executor

http::Response&	operator<<(http::Response&, Executor&);

#endif // EXECUTOR_HPP