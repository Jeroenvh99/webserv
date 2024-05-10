#ifndef LOGGING_HPP
# define LOGGING_HPP

# include "Client.hpp"

# include <array>
# include <iostream>
# include <string>
# include <utility>
# include <vector>

namespace logging {
	class Variable {
	public:
		enum class Type: unsigned char;
	
		Variable(Type);
		Variable(Type, std::string const&);
		Variable(Type, std::string&&);
		Variable(std::string const&);
		Variable(std::string&&);
	
		Type				type() const noexcept;
		std::string const&	data() const noexcept;
	private:
		Type		_type;
		std::string	_data;
	}; // class LogFormat::Element;
	
	enum class Variable::Type: unsigned char { // see NGINX docs for possible additions
		literal,
		time_local,
		request,
		status,
	}; // enum class Variable::Type

	using Format = std::vector<Variable>; // nest in AccessLogger?

	class Logger {
	public:
		Logger() = delete;
		Logger(std::ostream&);
		virtual ~Logger() = default;
	
		std::ostream&		os() const noexcept;
		std::string const&	timestamp() const noexcept;
		void				timestamp_update();
	private:
		std::ostream&	_os;
		std::string		_timestamp;
	}; // class Logger
	
	class AccessLogger: public Logger {
	public:
		AccessLogger();
		~AccessLogger() = default;
		AccessLogger(std::ostream&, Format const&);
		AccessLogger(std::ostream&, Format&&);
		AccessLogger(AccessLogger const&) = delete;
		AccessLogger(AccessLogger&&) = delete;
		AccessLogger&	operator=(AccessLogger const&) = delete;
		AccessLogger&	operator=(AccessLogger&&) = delete;

		void	log(Client const&);
	private:
		Format const	_fmt;
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
	
		void	log(std::string const&, Level);
	
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
}; // namespace logging

#endif // LOGGING_HPP
