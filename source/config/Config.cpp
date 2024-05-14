#include "Config.hpp"

const char* Config::InvalidSyntaxException::what() const throw()
{
	return "configException: syntax error in config file";
}

const char* Config::InvalidServerblockException::what() const throw()
{
	return "configException: invalid server block in configuration file";
}

Config::Config(std::string &filename) {
	std::ifstream in(filename);
	if (!in.is_open()) { 
        throw std::exception();
    }
	RemoveComments(in);
	Parse();
	in.close();
}

Config::Config(const Config& src) {
	*this = src;
}

Config &Config::operator=(const Config& src) {
	if (this != &src) {
		_errorlog = src.getErrorLog();
		_accesslog = src.getAccessLog();
		_servers = src.getServers();
	}
	return *this;
}

std::string loglevels[8] = {"debug", "info", "notice", "warn", "error", "crit", "alert", "emerg"};

t_serverlog Config::ParseLog(std::string &word, std::stringstream &s) {
	t_serverlog log = {"", logLevel::ERROR};
	std::getline(s, word, ' ');
	word.erase(word.find_last_not_of(";") + 1);
	log.filename = word;
	if (!s.eof()) {
		std::getline(s, word, ' ');
		word.erase(word.find_last_not_of(";") + 1);
		for (int i = 0; i <= 8; i++) {
			if (word == loglevels[i]) {
				log.level = static_cast<logLevel>(i);
				break;
			}
		}
	}
	return log;
}

void Config::Parse() {
	std::stringstream s(_config);
	std::string line;
	while (!s.eof()) {
		std::getline(s, line);
		std::string temp;
		std::stringstream linestream(line);
		std::getline(linestream, temp, ' ');
		if (temp == "error_log") {
			_errorlog = ParseLog(temp, linestream);
		} else if (temp == "access_log") {
			_accesslog = ParseLog(temp, linestream);
		} else if (temp == "server") {
			ParseServer(s); // return with s after the }
		}
	}
}

// void Config::parseLocation(std::string &previous, std::string &word, std::stringstream &s, t_config &server) {
// 	t_location loc {"", {}, server.allowedmethods};
// 	loc.path = previous + word;
// 	while (!s.eof()) {
// 		std::string line;
// 		std::getline(s, line);
// 		if (line == "}") {
// 			server.locations.push_back(loc);
// 			return;
// 		}
// 		std::stringstream linestream(line);
// 		std::string temp;
// 		std::getline(linestream, temp, ' ');
// 		if (temp == "location") {
// 			std::getline(linestream, temp, ' ');
// 			parseLocation(loc.path, temp, s, server);
// 		} else {
// 			std::string name = temp;
// 			while (1) {
// 				std::getline(linestream, temp, ' ');
// 				std::string value = temp;
// 				value.erase(value.find_last_not_of(";") + 1);
// 				loc.parameters.insert({name, value});
// 				if (temp.back() == ';') {
// 					break;
// 				}
// 			}
// 		}
// 	}
// }

void Config::ParseServer(std::stringstream &s) {
	t_config server = {_errorlog, _accesslog, -1, "", {}, {}, {http::RequestMethod::GET, http::RequestMethod::HEAD, http::RequestMethod::POST, http::RequestMethod::PUT, http::RequestMethod::DELETE, http::RequestMethod::CONNECT, http::RequestMethod::OPTIONS, http::RequestMethod::TRACE}};
	std::string line;
	while (!s.eof()) {
		std::getline(s, line);
		line.erase(0, line.find_first_not_of(' '));
		std::string temp;
		std::stringstream linestream(line);
		std::getline(linestream, temp, ' ');
		if (temp == "}") {
			_servers.push_back(server);
			return;
		}
		if (temp == "error_log") {
			server.errorlog = ParseLog(temp, linestream);
		} else if (temp == "access_log") {
			server.accesslog = ParseLog(temp, linestream);
		} else if (temp == "error_page") {
			std::vector<int> codes;
			while (1) {
				std::getline(linestream, temp, ' ');
				try {
					codes.push_back(std::stoi(temp));
				} catch(std::exception &e) {
					break;
				}
			}
			temp.pop_back();
			for (auto code : codes) {
				server.errorpages.insert(std::make_pair(code, temp));
			}
		}
		// else if (temp == "location") {
		// 	// std::vector<std::string> directives;
		// 	// while (1) {
		// 	// 	std::getline(linestream, temp, ' ');
		// 	// 	directives.push_back(temp);
		// 	// 	if (temp == "}") {
		// 	// 		break;
		// 	// 	}
		// 	// }
		// 	// while (1) {
		// 	// 	std::map<std::string, std::string> routes;
		// 	// 	std::string first;
		// 	// 	std::getline(linestream, first, ' ');
		// 	// 	if (first == '}') {
		// 	// 		server.locations.push_back(std::make_pair(directives, routes));
		// 	// 		break;
		// 	// 	}
		// 	// 	std::getline(linestream, temp, ' ');
		// 	// 	routes.push_back(std::make_pair(first, temp.pop_back()));
		// 	}
		// }
		else if (temp == "listen") {
			std::getline(linestream, temp, ' ');
			while (temp == "") {
				std::getline(linestream, temp, ' ');
			}
			temp.pop_back();
			server.port = std::stoi(temp);
		} else if (temp == "server_name") {
			std::getline(linestream, temp, ' ');
			temp.pop_back();
			server.servername = temp;
		}
	}
	throw Config::InvalidServerblockException();
}

void Config::RemoveComments(std::ifstream &in) {
	std::string line;
	std::string res = "";
	unsigned int openbrackets = 0;
	while (!in.eof()) {
		std::getline(in, line);
		size_t comment = line.find('#');
		if (comment != std::string::npos) {
			line.erase(comment);
		}
		size_t lastchar = line.find_last_not_of(" \n\r\t");
		line.erase(lastchar + 1);
		if (line == "" || lastchar == std::string::npos) {
			continue;
		}
		size_t open = line.find('{');
		while (open != std::string::npos) {
			++openbrackets;
			open = line.find('{', open + 1);
		}
		size_t close = line.find('}');
		while (close != std::string::npos) {
			--openbrackets;
			close = line.find('}', close + 1);
		}
		int size = line.length();
		for (int i = 0; i < size; i++) {
			if (line[i] == '\t') {
				line[i] = ' ';
			}
			if (i == size - 1 && line[i] != '{' && line[i] != '}' && line[i] != ';') {
				throw Config::InvalidSyntaxException();
			}
		}
		res += line + "\n";
	}
	if (openbrackets > 0) {
		throw Config::InvalidSyntaxException();
	}
	_config = res;
}

const t_serverlog &Config::getErrorLog() const {
	return _errorlog;
}

const t_serverlog &Config::getAccessLog() const {
	return _accesslog;
}

const std::vector<t_config> &Config::getServers() const {
	return _servers;
}

const std::string &Config::getConfig() const {
	return _config;
}

Config::~Config() {}

// int main() {
// 	try {
// 		std::string file = "/home/jvan-hal/Desktop/webserv/test/test5.conf";
// 		Config conf(file);
// 		std::cout << conf.getConfig();
// 	} catch (std::exception &e) {
// 		std::cerr << e.what();
// 	}
// }