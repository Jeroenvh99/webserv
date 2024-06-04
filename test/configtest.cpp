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
		cr_assert_str_eq(conf.getConfig().c_str(), "error_log logs/error.log debug;\naccess_log logs/access.log;\nserver {\n listen 80;\n server_name hello;\n}\n");
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
		Config::ServerLog errorlog = conf.getErrorLog();
		cr_assert_str_eq(errorlog.filename.c_str(), "logs/error.log");
		cr_assert_eq(errorlog.level, Config::LogLevel::DEBUG);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}

Test(parseconfig, config5) {
	try {
		std::string file = "test1.conf";
		Config conf(file);
		Config::Server server = conf.getServers()[0];
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
		// just see if {{} on one line works
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}

Test(parseconfig, config7) {
	try {
		std::string file = "test5.conf";
		Config conf(file);
		Config::Server server = conf.getServers()[0];
		std::unordered_map<int, std::string> expectederrpages = {std::make_pair(404, "/404.html"), std::make_pair(500, "/50x.html"), std::make_pair(502, "/50x.html"), std::make_pair(503, "/50x.html"), std::make_pair(504, "/50x.html")};
		for (int i = 0; i < 5; i++) {
			cr_assert(server.errorpages[i] == expectederrpages[i]);
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}

Test(parseconfig, config8) {
	try {
		std::string file = "test5.conf";
		Config conf(file);
		Config::Server server = conf.getServers()[0];
		std::vector<Config::Location> locations = server.locations;
		cr_assert(locations.size() == 3);
		cr_assert_str_eq(locations[0].path.c_str(), "/asa");
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}

Test(parseconfig, config9) {
	try {
		std::string file = "test5.conf";
		Config conf(file);
		Config::Server server = conf.getServers()[0];
		std::vector<http::Method> allowedmethods = server.allowedmethods;
		cr_assert(allowedmethods[http::Method::POST] == http::Method::NONE);
		cr_assert(allowedmethods[http::Method::DELETE] == http::Method::NONE);
		cr_assert(allowedmethods[http::Method::OPTIONS] == http::Method::NONE);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}

Test(parseconfig, config10) {
	try {
		std::string file = "test6.conf";
		Config conf(file);
		Config::Server server = conf.getServers()[0];
		std::vector<http::Method> allowedmethods = server.allowedmethods;
		cr_assert(allowedmethods[http::Method::HEAD] == http::Method::NONE);
		cr_assert(allowedmethods[http::Method::PUT] == http::Method::NONE);
		cr_assert(allowedmethods[http::Method::DELETE] == http::Method::NONE);
		cr_assert(allowedmethods[http::Method::CONNECT] == http::Method::NONE);
		cr_assert(allowedmethods[http::Method::OPTIONS] == http::Method::NONE);
		cr_assert(allowedmethods[http::Method::TRACE] == http::Method::NONE);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cr_assert(0);
	}
}

Test(parseconfig, config11) {
	try {
		std::string file = "test7.conf";
		Config conf(file);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

Test(parseconfig, config12) {
	try {
		std::string file = "configtest.cpp";
		Config conf(file);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}