#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "regexengine.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <exception>

void parseHeader(std::string& headers);

void redirect_stdout(void)
{
    cr_redirect_stdout();
}

TestSuite(misc, .init=redirect_stdout);

TestSuite(parserequest, .init=redirect_stdout);

Test(misc, letters1) {
	std::vector<std::string> v = { "dkhgadkjdu", "d[h-m]" };
	regexsearch(v);
	fflush(stdout);
    cr_assert_stdout_eq_str("dk\ndk\n");
}

Test(misc, numbers1) {
	std::vector<std::string> v = { "d1hgad6jdu", "d\\d" };
	regexsearch(v);
	fflush(stdout);
    cr_assert_stdout_eq_str("d1\nd6\n");
}

Test(misc, numbers2) {
	std::vector<std::string> v = { "d1hgad6jdu", "d\\D" };
	regexsearch(v);
	fflush(stdout);
    cr_assert_stdout_eq_str("du\n");
}

Test(misc, numbers3) {
	std::vector<std::string> v = { "d1hgad6jdu", "d[3-7]" };
	regexsearch(v);
	fflush(stdout);
    cr_assert_stdout_eq_str("d6\n");
}

Test(parserequest, headers1) {
	std::string in("Host: test.com\r\nContent-Type: app\r\n\r\n");
	try {
		parseHeader(in);
		fflush(stdout);
		cr_assert_stdout_eq_str("Name: Host Value: test.com\nName: Content-Type Value: app\n\n");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers2) {
	std::string in("Host: test.com\r\nContent-Type:\r\n");
	try {
		parseHeader(in);
		fflush(stdout);
		cr_assert_stdout_eq_str("Name: Host Value:  test.com\nContent-Type:  app\n\n");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}