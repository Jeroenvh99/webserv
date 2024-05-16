#ifndef CONFIG_C
# define CONFIG_C
# include <string>
# include <sstream>
# include <fstream>
# include <iostream> 
# include <unordered_map>
# include <vector>
# include <exception>
# include "http/Request.hpp"

enum logLevel {
	DEBUG,
	INFO,
	NOTICE,
	WARN,
	ERROR,
	CRIT,
	ALERT,
	EMERG
};

struct t_serverlog
{
	std::string filename;
	logLevel level;
};

struct t_location {
	std::string path;
	std::unordered_map<std::string, std::string> parameters;
	std::vector<http::RequestMethod> allowedmethods;
};

struct t_server {
	struct t_serverlog errorlog;
	struct t_serverlog accesslog;
	int port;
	std::string servername;
	std::unordered_map<int, std::string> errorpages;
	std::vector<t_location> locations;
	std::vector<http::RequestMethod> allowedmethods;
};

class Config {
	private:
		std::string _config;
		struct t_serverlog _errorlog;
		struct t_serverlog _accesslog;
		std::vector<struct t_server> _servers;
	public:
		Config(std::string &filename);
		Config(const Config& src);
		Config &operator=(const Config& src);
		t_serverlog ParseLog(std::string &word, std::stringstream &s);
		void ParseMethods(int allow, std::string &word, std::stringstream &linestream, std::vector<http::RequestMethod> &allowed);
		void ParseLocation(std::string &previousloc, std::string &word, std::stringstream &s, t_server &server);
		void Parse();
		void ParseServer(std::stringstream &s);
		void RemoveComments(std::ifstream &in);
		const t_serverlog &getErrorLog() const;
		const t_serverlog &getAccessLog() const;
		const std::vector<t_server> &getServers() const;
		const std::string &getConfig() const;
		~Config();
		class InvalidSyntaxException: public std::exception {
				virtual const char* what() const throw();
		};
};

#endif