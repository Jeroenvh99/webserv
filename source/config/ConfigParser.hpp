class Config {
	private:
		std::string _errorlog;
		std::string _accesslog;
		int _port;
		std::string _servername;
		std::map<std::vector<int>, std::string> _errorpages;
		std::map<std::string, std::map<std::string, std::string>> _locations;
}

class ServerConfig {
	private:
		Config _global;
		std::vector<Config> _locals;
}

void Parse(std::stringstream &s) {
	while (s.eof()) {
		std::getline(s, temp);
		if (temp[0] == '#') {
			state = COMMENT;
		}
	}
	//write values to global config, unless we're in SERVER state
}
