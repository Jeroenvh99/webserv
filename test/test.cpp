#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "regexengine.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <exception>

void parseHeader(std::string& headers);
void parseRequestLine(std::string& request);
void parseBody(int contentlength, std::string& request);

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

Test(misc, numbers4) {
	std::vector<std::string> v = { "d1hgad6jdu", "d[6]" };
	regexsearch(v);
	fflush(stdout);
    cr_assert_stdout_eq_str("d6\n");
}

Test(parserequest, requestline1) {
	std::string in("");
	try {
		parseRequestLine(in);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline2) {
	std::string in("GET ");
	try {
		parseRequestLine(in);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline3) {
	std::string in("GET www.test.com HTTP/1.0 hei\r");
	try {
		parseRequestLine(in);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline4) {
	std::string in("GET www.test.com HTTP/1.0");
	try {
		parseRequestLine(in);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline5) {
	std::string in("GET www.test.com HTTP/1.0\r\n");
	try {
		parseRequestLine(in);
		fflush(stdout);
		cr_assert_stdout_eq_str("Method: 0 Uri: www.test.com Httpversion: 0\n");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
		cr_assert(0);
	}
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
		cr_assert(0); // should fail
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers3) {
	std::string in("Host: test.com\r\nContent-Type:");
	try {
		parseHeader(in);
		fflush(stdout);
		cr_assert(0); // should fail
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers4) {
	std::string in("");
	try {
		parseHeader(in);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers5) {
	std::string in("\r\n");
	try {
		parseHeader(in);
		fflush(stdout);
		cr_assert_stdout_eq_str("\n");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
		cr_assert(0);
	}
}

Test(parserequest, body1) {
	std::string in("this is the body\r\n");
	try {
		parseBody(18, in);
		fflush(stdout);
		cr_assert_stdout_eq_str("this is the body\r\n");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for body\n";
		cr_assert(0);
	}
}

Test(parserequest, body2) {
	std::string in("this is the body\r\n");
	try {
		parseBody(14, in);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for body\n";
	}
}

Test(parserequest, body3) {
	std::string in("this is the\r\n body\r\n");
	try {
		parseBody(20, in);
		fflush(stdout);
		cr_assert_stdout_eq_str("this is the\r\n body\r\n");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for body\n";
		cr_assert(0);
	}
}

Test(parserequest, body4) {
	std::string in("this is the\r\n body\r\n");
	try {
		parseBody(13, in);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for body\n";
	}
}