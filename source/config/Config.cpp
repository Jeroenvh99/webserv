#include "Config.hpp"

const char* Config::InvalidSyntaxException::what() const throw()
{
	return "configException: syntax error in config file";
}

const char* Config::InvalidFileException::what() const throw()
{
	return "configException: invalid config file";
}

Config::Config(std::string &filename) {
	size_t extension = filename.find_last_of(".");
	if (extension == std::string::npos || filename.substr(extension + 1) != "conf") {
		throw Config::InvalidFileException();
	}
	std::ifstream in(filename);
	if (!in.is_open()) { 
        throw std::exception();
    }
	PreParse(in);
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

Config::ServerLog Config::ParseLog(std::string &word, std::stringstream &s) {
	ServerLog log = {"", logging::ErrorLogger::Level::error};
	std::getline(s, word, ' ');
	word.erase(word.find_last_not_of(";") + 1);
	log.filename = word;
	if (!s.eof()) {
		std::getline(s, word, ' ');
		word.erase(word.find_last_not_of(";") + 1);
		for (int i = 0; i <= 8; i++) {
			if (word == logging::ErrorLogger::_levels[i].second) {
				log.level = static_cast<logging::ErrorLogger::Level>(i);
				break;
			}
		}
	}
	return log;
}

void Config::ParseMethods(std::string &word, std::stringstream &linestream, std::vector<http::Method> &allowed) {
	int allow = 0;
	if (word.find("allow_") != std::string::npos) {
		allow = 1;
	}
	while (1) {
		std::getline(linestream, word, ' ');
		for (int i = 0; i < static_cast<int>(http::Method::NONE); i++) {
			if (word.find(http::methods[i].second) != std::string::npos) {
				if (!allow) {
					allowed[i] = http::Method::NONE;
				}
			} else if (allow) {
				allowed[i] = http::Method::NONE;
			}
		}
		if (word.find(';') != std::string::npos) {
			return;
		}
	}
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
			ParseServer(s);
		}
	}
}

void Config::ParseLocation(std::string &previousloc, std::string &word, std::stringstream &s, Server &server) {
	Location loc {previousloc, {}, server.allowedmethods};
	loc.path += word;
	while (1) {
		std::string line;
		std::getline(s, line);
		line.erase(0, line.find_first_not_of(' '));
		if (line.find('}') != std::string::npos) {
			server.locations.push_back(loc);
			return;
		}
		std::stringstream linestream(line);
		std::string temp;
		std::getline(linestream, temp, ' ');
		if (temp == "location") {
			std::getline(linestream, temp, ' ');
			ParseLocation(loc.path, temp, s, server);
		} else if (temp == "allow_methods" || temp == "deny_methods"){
			ParseMethods(temp, linestream, loc.allowedmethods);
		} else {
			std::string name = temp;
			while (1) {
				std::getline(linestream, temp, ' ');
				std::string value = temp;
				value.erase(value.find_last_not_of(';') + 1);
				loc.parameters.insert({name, value});
				if (temp.back() == ';') {
					break;
				}
			}
		}
	}
}

void Config::ParseServer(std::stringstream &s) {
	Server server = {_errorlog, _accesslog, -1, "", {}, {}, {http::Method::GET, http::Method::HEAD, http::Method::POST, http::Method::PUT, http::Method::DELETE, http::Method::CONNECT, http::Method::OPTIONS, http::Method::TRACE}};
	std::string line;
	while (!s.eof()) {
		std::getline(s, line);
		line.erase(0, line.find_first_not_of(' '));
		std::string temp;
		std::stringstream linestream(line);
		std::getline(linestream, temp, ' ');
		if (temp == "}") {
			if (server.port == -1) {
				server.port = 1100;
			}
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
		} else if (temp == "location") {
			std::getline(linestream, temp, ' ');
			std::string loc = "";
			ParseLocation(loc, temp, s, server);
		} else if (temp == "allow_methods" || temp == "deny_methods"){
			ParseMethods(temp, linestream, server.allowedmethods);
		} else if (temp == "listen") {
			std::getline(linestream, temp, ' ');
			temp.pop_back();
			server.port = std::stoi(temp);
		} else if (temp == "server_name") {
			std::getline(linestream, temp, ' ');
			temp.pop_back();
			server.servername = temp;
		}
	}
}

void Config::PreParse(std::ifstream &in) {
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
		if (line == "" || lastchar == std::string::npos) {
			continue;
		}
		line.erase(lastchar + 1);
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
		for (size_t i = 0; i < line.length(); i++) {
			if (line[i] == '\t') {
				line[i] = ' ';
			}
			if (i > 0 && line[i - 1] == ' ' && line[i] == ' ') {
				line.erase(i, 1);
				--i;
			}
		}
		if (line.back() != '{' && line.back() != '}' && line.back() != ';') {
			throw Config::InvalidSyntaxException();
		}
		res += line + "\n";
	}
	if (openbrackets > 0) {
		throw Config::InvalidSyntaxException();
	}
	_config = res;
}

const Config::ServerLog &Config::getErrorLog() const {
	return _errorlog;
}

const Config::ServerLog &Config::getAccessLog() const {
	return _accesslog;
}

const std::vector<Config::Server> &Config::getServers() const {
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
// 		Server server = conf.getServers()[0];
// 	} catch (std::exception &e) {
// 		std::cerr << e.what();
// 	}
// }