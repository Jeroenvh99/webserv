#ifndef LOGGING_HPP
#define LOGGING_HPP

#include "Client.hpp"

#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace logging {

class Variable {
public:
	enum class Type: unsigned char { // see NGINX docs for possible additions
		literal,
		time_local,
		request,
		status,
		host,
		client
	}; // enum class Type

	Variable(Type = Type::literal);
	Variable(Type, std::string const&);
	Variable(Type, std::string&&);
	Variable(std::string const&);
	Variable(std::string&&);

	Type 				type() const noexcept;
	std::string const&	data() const noexcept;

private:
	Type		_type;
	std::string	_data;
}; // class Variable;

class Logger {
public:
	~Logger();

	std::ostream&		os() noexcept;
	void				attach_file(std::string const&);
	std::string const&	timestamp() const noexcept;
	void				timestamp_update();

private:
	friend class AccessLogger;
	friend class ErrorLogger;

	Logger(std::ostream&);

	void	detach_file() noexcept;

	std::ostream*	_os;
	std::string		_timestamp;
}; // class Logger

class AccessLogger: public Logger {
public:
	using Format = std::vector<Variable>;

	AccessLogger();

	Format&			format() noexcept;
	Format const&	format() const noexcept;

	void	log(Client const&, VirtualServer const&);

	static const Format	default_fmt;

private:
	Format	_fmt;
}; // class AccessLogger

class ErrorLogger: public Logger {
public:
	enum class Level: int {
		debug,
		info,
		notice,
		warning,
		error,
		critical,
		alert,
		emergency,
	}; // enum Level
	using Literals = std::array<std::pair<Level, char const *>, 8>;

	ErrorLogger();

	Level&	level() noexcept;
	Level	level() const noexcept;

	template<typename... Ts>
	void log(Level, Ts...);

	static char const*			level_to_string(Level);
	static ErrorLogger::Level	level_from_string(std::string const&);

	static constexpr Level	default_level = Level::error;
	static const Literals	literals;

private:
	Level	_level;
}; // class ErrorLogger

inline const AccessLogger::Format		AccessLogger::default_fmt = {
	{"["},
	{Variable::Type::time_local},
	{"] processed a request :)"},
	{" this server is called: "},
	{Variable::Type::host},
	{" and the clients address is: "},
	{Variable::Type::client},
	{" with request: "},
	{Variable::Type::request},
};
inline constexpr ErrorLogger::Literals	ErrorLogger::literals = {{
	{Level::debug, "debug"},
	{Level::info, "info"},
	{Level::notice, "notice"},
	{Level::warning, "warning"},
	{Level::error, "error"},
	{Level::critical, "critical"},
	{Level::alert, "alert"},
	{Level::emergency, "emergency"},
}};

extern AccessLogger	alog;
extern ErrorLogger	elog;

}; // namespace logging

#include "logging/ErrorLogger.tpp"

#endif // LOGGING_HPP
