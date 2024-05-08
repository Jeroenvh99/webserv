#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "regexengine.hpp"
#include <vector>
#include <string>
#include <iostream>

using namespace http;

void redirect_stdout(void)
{
    cr_redirect_stdout();
}

TestSuite(regexengine, .init=redirect_stdout);

Test(regexengine, letters1) {
	std::vector<std::string> v = { "dkhgadkjdu", "d[h-m]" };
	regexsearch(v);
	fflush(stdout);
    cr_assert_stdout_eq_str("dk\ndk\n");
}

Test(regexengine, numbers1) {
	std::vector<std::string> v = { "d1hgad6jdu", "d\\d" };
	regexsearch(v);
	fflush(stdout);
    cr_assert_stdout_eq_str("d1\nd6\n");
}

Test(regexengine, numbers2) {
	std::vector<std::string> v = { "d1hgad6jdu", "d\\D" };
	regexsearch(v);
	fflush(stdout);
    cr_assert_stdout_eq_str("du\n");
}

Test(regexengine, numbers3) {
	std::vector<std::string> v = { "d1hgad6jdu", "d[3-7]" };
	regexsearch(v);
	fflush(stdout);
    cr_assert_stdout_eq_str("d6\n");
}

Test(regexengine, numbers4) {
	std::vector<std::string> v = { "d1hgad6jdu", "d[6]" };
	regexsearch(v);
	fflush(stdout);
    cr_assert_stdout_eq_str("d6\n");
}