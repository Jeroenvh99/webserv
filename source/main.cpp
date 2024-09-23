#include "Server.hpp"
#include "Config.hpp"
#include "Environment.hpp"
#include "Poller.hpp"

#include <iostream>
#include <fstream>

static constexpr int		dfl_backlog_size = 5192;

static size_t	_get_cenvsize(char const* const*);

int
main(int argc, char** argv, char** envp) {
	Environment::_parent_env = envp;
	Environment::_parent_env_size = _get_cenvsize(envp);
	bool	duplicate = false;

	if (argc > 2)
		return (std::cerr << "Usage: ./webserv [path_to_config]\n", 1);
	try {
		std::string file;
		if (argc == 2) {
			file = argv[1];
		} else {
			file = "configs/default.conf";
		}
		Config	conf(file);
		auto serverconfigs = conf.getServers();
		std::vector<Server> servers;

		for (size_t i = 0; i < serverconfigs.size(); i++) {
			for (size_t j = 0; j < i; j++) {
				if (serverconfigs[j].port == serverconfigs[i].port)
					duplicate = true;
			}
			if (duplicate == false) {
				std::ofstream accessFile;
				if(serverconfigs[i].accesslog.filename != "")
					accessFile.open(serverconfigs[i].accesslog.filename, std::ios::out);
				std::ostream& access = (serverconfigs[i].accesslog.filename != ""? accessFile : std::cout);
				std::ofstream errorFile;
				if(serverconfigs[i].errorlog.filename != "")
					errorFile.open(serverconfigs[i].errorlog.filename, std::ios::out);
				std::ostream& error = (serverconfigs[i].errorlog.filename != ""? errorFile : std::cerr);
				Server serv(serverconfigs[i], dfl_backlog_size, access, error);
				for (size_t j = 0; j < serverconfigs.size(); j++) {
					if (serverconfigs[j].port == serverconfigs[i].port) {
						serv.virtual_server_add(serverconfigs[j]);
					}
				}
				servers.emplace_back(std::move(serv));
			}
			duplicate = false;
			if (i == serverconfigs.size() - 1) {
				while (true) {
					g_poller.wait();
					for (size_t j = 0; j < servers.size(); j++) {
						servers[j].process();
					}
				}
			}
		}
	} catch (std::exception& e) {
		return (std::cerr << "webserv: " << e.what() << '\n', 1);
	}

    std::cout << "webserv: exiting\n";

	return (0);
}

static size_t	_get_cenvsize(char const* const* envp) {
	size_t	size = 0;

	while (envp[size] != nullptr)
		++size;
	return (size);
}
