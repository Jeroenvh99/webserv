#ifndef CONFIG_C
# define CONFIG_C
# include <string>
# include <sstream>
# include <fstream>
# include <iostream> 
# include <unordered_map>
# include <vector>
# include <exception>

namespace configUtils {
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

	// std::string loglevels[8] = {"debug", "info", "notice", "warn", "error", "crit", "alert", "emerg"};

	struct t_serverlog
	{
		std::string filename;
		logLevel level;
	};

	struct t_location {
		std::string path;
		std::unordered_map<std::string, std::string> parameters;
		std::vector<std::string> allowedmethods;
	};

	struct t_config {
		struct t_serverlog errorlog;
		struct t_serverlog accesslog;
		int port;
		std::string servername;
		std::unordered_map<int, std::string> errorpages;
		std::vector<t_location> locations; // can be nested
		std::vector<std::string> allowedmethods;
	};
}

class Config {
	private:
		std::string _config;
		struct configUtils::t_serverlog _errorlog;
		struct configUtils::t_serverlog _accesslog;
		std::vector<struct configUtils::t_config> _locals;
	public:
		Config(std::string &filename);
		Config(const Config& src);
		Config &operator=(const Config& src);
		// t_serverlog ParseLog(std::string &word, std::stringstream &s);
		// void Parse(std::stringstream &s);
		// void parseServer(std::stringstream &s);
		void removeComments(std::ifstream &in);
		const configUtils::t_serverlog &getErrorLog() const;
		const configUtils::t_serverlog &getAccessLog() const;
		const std::vector<configUtils::t_config> &getLocals() const;
		const std::string &getConfig() const;
		~Config();
		class InvalidSyntaxException: public std::exception {
			virtual const char* what() const throw();
		};
		class InvalidServerblockException: public std::exception {
			virtual const char* what() const throw();
		};
};

#endif