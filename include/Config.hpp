#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <exception>

#include "http/Request.hpp"
#include "logging.hpp"

class Config
{
public:
	struct ServerLog
	{
		std::string filename;
		logging::ErrorLogger::Level level;
	};

	struct Location
	{
		std::string path;
		std::unordered_map<std::string, std::string> parameters;
		std::vector<http::Method> allowedmethods;
	};

	struct Server
	{
		struct ServerLog errorlog;
		struct ServerLog accesslog;
		int port;
		std::string servername;
		std::unordered_map<int, std::string> errorpages;
		std::vector<Location> locations;
		std::vector<http::Method> allowedmethods;
	};

	class InvalidSyntaxException : public std::exception
	{
		virtual const char *what() const throw();
	};

	class InvalidFileException : public std::exception
	{
		virtual const char *what() const throw();
	};

	Config(std::string &filename);
	Config(const Config &src);
	~Config();
	Config &operator=(const Config &src);

	ServerLog ParseLog(std::string &word, std::stringstream &s);
	void ParseMethods(std::string &word, std::stringstream &linestream, std::vector<http::Method> &allowed);
	void ParseLocation(std::string &previousloc, std::string &word, std::stringstream &s, Server &server);
	void Parse();
	void ParseServer(std::stringstream &s);
	void PreParse(std::ifstream &in);
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
