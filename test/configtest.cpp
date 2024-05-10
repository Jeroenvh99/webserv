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
		cr_assert_str_eq(conf.getConfig().c_str(), "error_log\tlogs/error.log;\n"
"server {\n"
"\tlisten\t\t80;\n"
"\terror_page\t404\t/404.html;\n"
"\tlocation /404.html /50x.html {\n"
"\t\troot\t./tests/default/error;\n"
"\t}\n"
"}\n");
	} catch (std::exception &e) {
		std::cerr << e.what();
		cr_assert(0);
	}
}

Test(parseconfig, config2) {
	try {
		std::string file = "test2.conf";
		Config conf(file);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << e.what();
	}
}

Test(parseconfig, config3) {
	try {
		std::string file = "test3.conf";
		Config conf(file);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << e.what();
	}
}