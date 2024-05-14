#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "Config.hpp"
#include <string>
#include <iostream>
#include <exception>

static void redirect_stdout(void)
{
    cr_redirect_stdout();
}

TestSuite(parseconfig, .init=redirect_stdout);

Test(parseconfig, config1) {
	try {
		std::string file = "test1.conf";
		Config conf(file);
		cr_assert_str_eq(conf.getConfig().c_str(), "error_log logs/error.log debug;\naccess_log logs/access.log;\nserver {\n listen  80;\n server_name hello;\n}\n");
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}

Test(parseconfig, config2) {
	try {
		std::string file = "test2.conf";
		Config conf(file);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

Test(parseconfig, config3) {
	try {
		std::string file = "test3.conf";
		Config conf(file);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

Test(parseconfig, config4) {
	try {
		std::string file = "test1.conf";
		Config conf(file);
		t_serverlog errorlog = conf.getErrorLog();
		cr_assert_str_eq(errorlog.filename.c_str(), "logs/error.log");
		cr_assert_eq(errorlog.level, logLevel::DEBUG);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}

Test(parseconfig, config5) {
	try {
		std::string file = "test1.conf";
		Config conf(file);
		t_config server = conf.getServers()[0];
		cr_assert_str_eq(server.accesslog.filename.c_str(), "logs/access.log");
		cr_assert_str_eq(server.servername.c_str(), "hello");
		cr_assert_eq(server.port, 80);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}

Test(parseconfig, config6) {
	try {
		std::string file = "test4.conf";
		Config conf(file);
		// t_config server = conf.getServers()[0];
		// cr_assert_str_eq(server.accesslog.filename.c_str(), "logs/access.log");
		// cr_assert_str_eq(server.servername.c_str(), "hello");
		// cr_assert_eq(server.port, 80);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}

Test(parseconfig, config7) {
	try {
		std::string file = "test4.conf";
		Config conf(file);
		t_config server = conf.getServers()[0];
		std::unordered_map<int, std::string> expectederrpages = {std::make_pair(404, "/404.html"), std::make_pair(500, "/50x.html"), std::make_pair(502, "/50x.html"), std::make_pair(503, "/50x.html"), std::make_pair(504, "/50x.html")};
		for (int i = 0; i < 5; i++) {
			cr_assert(server.errorpages[i] == expectederrpages[i]);
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}