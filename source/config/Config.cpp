#include "Config.hpp"
#include <string.h>

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
	try {
		removeComments(in);
		in.close();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		in.close();
	}
}

Config::Config(const Config& src) {
	*this = src;
}

Config &Config::operator=(const Config& src) {
	if (this != &src) {
		_errorlog = src.getErrorLog();
		_accesslog = src.getAccessLog();
		_locals = src.getLocals();
	}
	return *this;
}

// std::string loglevels[8] = {"debug", "info", "notice", "warn", "error", "crit", "alert", "emerg"};

// t_serverlog Config::ParseLog(std::string &word, std::stringstream &s) {
// 	t_serverlog log = {"", logLevel::ERROR};
// 	std::getline(s, word, ' ');
// 	if (s.eof() && word.back() != ';') {
// 		throw Config::InvalidSyntaxException();
// 	}
// 	word.erase(word.find_last_not_of(";") + 1);
// 	log.filename = word;
// 	if (s.eof()) {
// 		return log;
// 	}
// 	std::getline(s, word, ' ');
// 	if (word.back() != ';') {
// 		throw Config::InvalidSyntaxException();
// 	}
// 	for (int i = 0; i <= 8; i++) {
// 		if (word == levels[i]) {
// 			log.level = static_cast<logLevel>(i);
// 			break;
// 		}
// 	}int main() {
// 	return log;
// }

// void Config::Parse(std::stringstream &s) {
// 	std::string line;
// 	while (!s.eof()) {
// 		std::getline(s, line);
// 		if (line[0] == '#') {
// 			continue; //skip entire comment-lines
// 		}
// 		if (line.back() != ';') {
// 			throw Config::InvalidSyntaxException();
// 		}
// 		std::string temp;
// 		std::stringstream linestream(line);
// 		std::getline(linestream, temp, ' ');
// 		if (temp == "error_log") {
// 			_errorlog = ParseLog(temp, linestream);
// 		} else if (temp == "access_log") {
// 			_accesslog = ParseLog(temp, linestream);
// 		} else if (temp == "server") {
// 			parseServer(s); // return with s after the }
// 		}
// 	}
// }

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

// void Config::parseServer(std::stringstream &s) {
// 	t_config server = {_errorlog, _accesslog, -1, "", {}, {}, {requestType::GET, requestType::HEAD, requestType::POST, requestType::PUT, requestType::DELETE, requestType::CONNECT, requestType::OPTIONS, requestType::TRACE}};
// 	std::string line;
// 	while (s.eof()) {
// 		std::getline(s, line);
// 		if (line[0] == '}') {
// 			_locals.push_back(server);
// 			return;
// 		}
// 		std::string temp;
// 		std::stringstream linestream(line);
// 		std::getline(linestream, temp, ' ');
// 		if (temp == "error_log") {
// 			server.errorlog = ParseLog(temp, linestream);
// 		} else if (temp == "access_log") {
// 			server.accesslog = ParseLog(temp, linestream);
// 		} else if (temp == "error_page") {
// 			std::vector<int> codes;
// 			while (1) {
// 				std::getline(linestream, temp, ' ');
// 				try {
// 					codes.push_back(std::stoi(temp));
// 				} catch(std::exception &e) {
// 					break;
// 				}
// 			}
// 			if (temp.back() != ';') {
// 				throw Config::InvalidSyntaxException();
// 			}
// 			temp.pop_back();
// 			for (auto code : codes) {
// 				server.errorpages.insert(std::make_pair(code, temp));
// 			}
// 		} else if (temp == "location") {
// 			// std::vector<std::string> directives;
// 			// while (1) {
// 			// 	std::getline(linestream, temp, ' ');
// 			// 	directives.push_back(temp);
// 			// 	if (temp == "}") {
// 			// 		break;
// 			// 	}
// 			// }
// 			// while (1) {
// 			// 	std::map<std::string, std::string> routes;
// 			// 	std::string first;
// 			// 	std::getline(linestream, first, ' ');
// 			// 	if (first == '}') {
// 			// 		server.locations.push_back(std::make_pair(directives, routes));
// 			// 		break;
// 			// 	}
// 			// 	std::getline(linestream, temp, ' ');
// 			// 	if (temp.back() != ';') {
// 			// 		throw Config::InvalidSyntaxException();
// 			// 	}
// 			// 	routes.push_back(std::make_pair(first, temp.pop_back()));
// 			}
// 		}
// 		else if (temp == "listen") {
// 			std::getline(linestream, temp, ' ');
// 			server.port = std::stoi(temp.pop_back());
// 		} else if (temp == "server_name") {
// 			std::getline(linestream, temp, ' ');
// 			server.servername = temp.pop_back();
// 		}
// 	}
// 	throw Config::InvalidServerblockException();
// }

void Config::removeComments(std::ifstream &in) {
	std::string line;
	std::string res = "";
	int openbrackets = 0;
	while (!in.eof()) {
		std::getline(in, line);
		if (line[0] == '#') {
			continue;
		}
		size_t comment = line.find('#');
		if (comment != std::string::npos) {
			line.erase(comment);
		}
		if (line.find('{') != std::string::npos) {
			++openbrackets;
		}
		if (line.find('}') != std::string::npos) {
			--openbrackets;
		}
		line.erase(line.find_last_not_of(" \n\r\t") + 1);
		if (line == "") {
			continue;
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

const configUtils::t_serverlog &Config::getErrorLog() const {
	return _errorlog;
}

const configUtils::t_serverlog &Config::getAccessLog() const {
	return _accesslog;
}

const std::vector<configUtils::t_config> &Config::getLocals() const {
	return _locals;
}

const std::string &Config::getConfig() const {
	return _config;
}

Config::~Config() {}

// int main() {
// 	std::string file = "/home/jvan-hal/Desktop/webserv/test/test1.conf";
// 	Config test(file);
// 	const char *res = test.getConfig().c_str();
// 	size_t where = strcmp(res, "error_log\tlogs/error.log;\n"
// "server {\n"
// "\tlisten\t\t80;\n"
// "\terror_page\t404\t/404.html;\n"
// "\tlocation /404.html /50x.html {\n"
// "\t\troot\t./tests/default/error;\n"
// "\t}\n"
// "}\n");
// 	std::cout << res[where] << where;
// }