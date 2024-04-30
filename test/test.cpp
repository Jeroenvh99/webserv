#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "regexengine.hpp"
#include "HttpRequest.hpp"
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <iostream>
#include <exception>

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
		std::stringstream s(in);
		HttpRequest req;
		req.parseRequestLine(s);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline2) {
	std::string in("GET ");
	try {
		std::stringstream s(in);
		HttpRequest req;
		req.parseRequestLine(s);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline3) {
	std::string in("GET www.test.com HTTP/1.0 hei\r");
	try {
		std::stringstream s(in);
		HttpRequest req;
		req.parseRequestLine(s);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline4) {
	std::string in("GET www.test.com HTTP/1.0");
	try {
		std::stringstream s(in);
		HttpRequest req;
		req.parseRequestLine(s);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline5) {
	std::string in("GET www.test.com HTTP/1.0\r\n");
	try {
		std::stringstream s(in);
		HttpRequest req;
		req.parseRequestLine(s);
		fflush(stdout);
		cr_assert_str_eq(req.getRequestUri().c_str(), "www.test.com");
		cr_assert_eq(req.getRequestType(), requestType::GET);
		cr_assert_eq(req.getHttpVersion(), httpVersion::ONE);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
		cr_assert(0);
	}
}

Test(parserequest, headers1) {
	std::string in("Host: test.com\r\nContent-Type: app\r\n\r\n");
	try {
		std::stringstream s(in);
		HttpRequest req;
		req.parseHeaders(s);
		fflush(stdout);
		std::unordered_map<std::string, std::string> headers = req.getHeaders();
		cr_assert_str_eq(headers["Host"].c_str(), "test.com");
		cr_assert_str_eq(headers["Content-Type"].c_str(), "app");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers2) {
	std::string in("Host: test.com\r\nContent-Type:\r\n");
	try {
		std::stringstream s(in);
		HttpRequest req;
		req.parseHeaders(s);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers3) {
	std::string in("Host: test.com\r\nContent-Type:");
	try {
		std::stringstream s(in);
		HttpRequest req;
		req.parseHeaders(s);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers4) {
	std::string in("");
	try {
		std::stringstream s(in);
		HttpRequest req;
		req.parseHeaders(s);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers5) {
	std::string in("\r\n");
	try {
		std::stringstream s(in);
		HttpRequest req;
		req.parseHeaders(s);
		fflush(stdout);
		std::unordered_map<std::string, std::string> headers = req.getHeaders();
		cr_assert_eq(headers.size(), 0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
		cr_assert(0);
	}
}

Test(parserequest, body1) {
	std::string in("this is the body\r\n");
	try {
		std::stringstream s(in);
		HttpRequest req(18);
		req.parseBody(s);
		fflush(stdout);
		cr_assert_str_eq(req.getMessage().c_str(), "this is the body\r\n");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for body\n";
		cr_assert(0);
	}
}

Test(parserequest, body2) {
	std::string in("this is the body\r\n");
	try {
		std::stringstream s(in);
		HttpRequest req(14);
		req.parseBody(s);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for body\n";
	}
}

Test(parserequest, body3) {
	std::string in("this is the\r\n body\r\n");
	try {
		std::stringstream s(in);
		HttpRequest req(20);
		req.parseBody(s);
		fflush(stdout);
		cr_assert_str_eq(req.getMessage().c_str(), "this is the\r\n body\r\n");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for body\n";
		cr_assert(0);
	}
}

Test(parserequest, body4) {
	std::string in("this is the\r\n body\r\n");
	try {
		std::stringstream s(in);
		HttpRequest req(14);
		req.parseBody(s);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for body\n";
	}
}

Test(parserequest, complete1) {
	std::string in("\r\n");
	try {
		std::array<char, 512> s = {0};
		std::copy(in.begin(), in.end(), s.begin());
		HttpRequest req;
		req.addBuffer(s);
		std::fill(s.begin(), s.end(), 0);
		req.addBuffer(s);
		fflush(stdout);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for request\n";
	}
}