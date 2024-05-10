#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <array>
# include <iostream>
# include <string>
# include <utility>

class Logger {
public:
	Logger() = delete;
	Logger(std::ostream&);
	virtual ~Logger() = default;

	std::ostream&	os() noexcept;

	void	log(std::string const&) const;
	void	timestamp() const;

private:
	std::ostream&	_os;
}; // class Logger

class AccessLogger: public Logger {
public:
	AccessLogger();
	~AccessLogger() = default;
	AccessLogger(std::ostream&);
	AccessLogger(AccessLogger const&) = delete;
	AccessLogger(AccessLogger&&) = delete;
	AccessLogger&	operator=(AccessLogger const&) = delete;
	AccessLogger&	operator=(AccessLogger&&) = delete;
}; // class AccessLogger

class ErrorLogger: public Logger {
public:
	enum Level: int;

	ErrorLogger();
	~ErrorLogger() = default;
	ErrorLogger(std::ostream&, Level);
	ErrorLogger(ErrorLogger const&) = delete;
	ErrorLogger(ErrorLogger&&) = delete;
	ErrorLogger&	operator=(ErrorLogger const&) = delete;
	ErrorLogger&	operator=(ErrorLogger&&) = delete;

	void	log(std::string const&, Level) const;

	static char const*			level_to_string(ErrorLogger::Level);
	static ErrorLogger::Level	level_from_string(std::string const&);

private:
	using LevelStringMap = std::array<std::pair<Level, char const*>, 8>;

	static const LevelStringMap	_levels;

	Level const		_level;
}; // class ErrorLogger

enum ErrorLogger::Level: int {
	debug,
	info,
	notice,
	warning,
	error,
	critical,
	alert,
	emergency,
}; // enum ErrorLogger::Level

inline constexpr ErrorLogger::LevelStringMap	ErrorLogger::_levels = {{
	{debug, "debug"},
	{info, "info"},
	{notice, "notice"},
	{warning, "warning"},
	{error, "error"},
	{critical, "critical"},
	{alert, "alert"},
	{emergency, "emergency"},
}};

#endif // LOGGER_HPP
