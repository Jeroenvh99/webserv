#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <exception>

#include "logging/logging.hpp"

class Config
{
public:
	using MethodBitmask = std::underlying_type<http::Method>::type;
	struct ServerLog
	{
		std::string filename;
		logging::ErrorLogger::Level level;
	};

	struct Location
	{
		std::vector<std::string> paths;
		std::vector<std::string> allowedcgi;
		std::string root;
		std::string index;
		int maxbodysize;
		std::unordered_map<std::string, std::string> parameters;
		MethodBitmask allowedmethods;
	};

	struct Redirection {
		std::string from;
		std::string to;
		bool permanent;
	};

	struct Server
	{
		void AddErrorPage(std::stringstream &);
		void AddRedirect(std::stringstream &);
		int port;
		int maxbodysize;
		std::string servername;
		std::unordered_map<int, std::string> errorpages;
		std::vector<Location> locations;
		std::vector<Redirection> redirections;
		MethodBitmask allowedmethods;
	};

	class InvalidSyntaxException : public std::exception
	{
		virtual const char *what() const throw();
	};

	class InvalidFileException : public std::exception
	{
		virtual const char *what() const throw();
	};

	Config(std::string &);
	Config(const Config &);
	~Config();
	Config &operator=(const Config &);

	ServerLog ParseLog(std::string &, std::stringstream &);
	void ParseMethods(std::string &, std::stringstream &, MethodBitmask &);
	void ParseLocation(std::vector<std::string> &, std::string &, std::stringstream &, std::stringstream &, Server &, MethodBitmask &);
	static int ParseBodySize(std::stringstream &);
	void Parse();
	void ParseServer(std::stringstream &);
	void PreParse(std::ifstream &);
	void MatchErrorPages();
	const ServerLog &getErrorLog() const;
	const ServerLog &getAccessLog() const;
	const std::vector<Server> &getServers() const;
	const std::string &getConfig() const;

private:
	std::string _config;
	ServerLog _errorlog;
	ServerLog _accesslog;
	std::vector<Server> _servers;
};
