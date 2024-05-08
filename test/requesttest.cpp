#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "regexengine.hpp"
<<<<<<< HEAD:test/requesttest.cpp
#include "HttpRequest.hpp"
#include "Config.hpp"
=======
#include "Request.hpp"
>>>>>>> main:test/test.cpp
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <iostream>
#include <exception>

using namespace http;

void redirect_stdout(void)
{
    cr_redirect_stdout();
}

TestSuite(misc, .init=redirect_stdout);

TestSuite(parserequest, .init=redirect_stdout);

TestSuite(parseconfig, .init=redirect_stdout);

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
		Request req;
		req.parseRequestLine(s);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline2) {
	std::string in("GET ");
	try {
		std::stringstream s(in);
		Request req;
		req.parseRequestLine(s);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline3) {
	std::string in("GET www.test.com HTTP/1.0 hei\r");
	try {
		std::stringstream s(in);
		Request req;
		req.parseRequestLine(s);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline4) {
	std::string in("GET www.test.com HTTP/1.0");
	try {
		std::stringstream s(in);
		Request req;
		req.parseRequestLine(s);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
	}
}

Test(parserequest, requestline5) {
	std::string in("GET www.test.com HTTP/1.0\r\n");
	try {
		std::stringstream s(in);
		Request req;
		req.parseRequestLine(s);
		cr_assert_str_eq(req.getRequestUri().c_str(), "www.test.com");
		cr_assert_eq(req.getMethod(), RequestMethod::GET);
		cr_assert_eq(req.getHttpVersion(), HttpVersion::ONE);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for requestline\n";
		cr_assert(0);
	}
}

Test(parserequest, headers1) {
	std::string in("Host: test.com\r\nContent-Type: app\r\n\r\n");
	try {
		std::stringstream s(in);
		Request req;
		req.parseHeaders(s);
		std::unordered_multimap<std::string, std::string> headers = req.getHeaders();
		cr_assert_str_eq(headers.find("Host")->second.c_str(), "test.com");
		cr_assert_str_eq(headers.find("Content-Type")->second.c_str(), "app");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
		cr_assert(0);
	}
}

Test(parserequest, headers2) {
	std::string in("Host: test.com\r\nContent-Type:\r\n");
	try {
		std::stringstream s(in);
		Request req;
		req.parseHeaders(s);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers3) {
	std::string in("Host: test.com\r\nContent-Type:");
	try {
		std::stringstream s(in);
		Request req;
		req.parseHeaders(s);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers4) {
	std::string in("");
	try {
		std::stringstream s(in);
		Request req;
		req.parseHeaders(s);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
	}
}

Test(parserequest, headers5) {
	std::string in("\r\n");
	try {
		std::stringstream s(in);
		Request req;
		req.parseHeaders(s);
		std::unordered_multimap<std::string, std::string> headers = req.getHeaders();
		cr_assert_eq(headers.size(), 0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
		cr_assert(0);
	}
}

Test(parserequest, headers6) {
	std::string in("Host: test.com\r\nContent-Length: 25\r\n\r\n");
	try {
		std::stringstream s(in);
		Request req;
		req.parseHeaders(s);
		std::unordered_multimap<std::string, std::string> headers = req.getHeaders();
		cr_assert_str_eq(headers.find("Host")->second.c_str(), "test.com");
		cr_assert_str_eq(headers.find("Content-Length")->second.c_str(), "25");
		cr_assert_eq(req.getContentLength(), 25);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for header\n";
		cr_assert(0);
	}
}

Test(parserequest, body1) {
	std::string in("this is the body\r\n");
	try {
		std::stringstream s(in);
		Request req(18);
		req.parseBody(s);
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
		Request req(14);
		req.parseBody(s);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for body\n";
	}
}

Test(parserequest, body3) {
	std::string in("this is the\r\n body\r\n");
	try {
		std::stringstream s(in);
		Request req(20);
		req.parseBody(s);
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
		Request req(14);
		req.parseBody(s);
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
		Request req;
		req.addBuffer(s);
		std::fill(s.begin(), s.end(), 0);
		req.addBuffer(s);
		cr_assert(0);
	} catch (std::exception &e) {
		std::cerr << "incorrect format for request\n";
	}
}

Test(parserequest, complete2) {
	std::string in("GET www.test.com HTTP/1.0\r\nHost: test.com\r\nContent-Type: app\r\nContent-Length: 18\r\n\r\nthis is the body\r\n");
	try {
		std::array<char, 512> s = {0};
		std::copy(in.begin(), in.end(), s.begin());
		Request req;
		req.addBuffer(s);
		std::fill(s.begin(), s.end(), 0);
		req.addBuffer(s);
		std::unordered_multimap<std::string, std::string> headers = req.getHeaders();
		cr_assert_str_eq(req.getRequestUri().c_str(), "www.test.com");
		cr_assert_eq(req.getRequestType(), requestType::GET);
		cr_assert_eq(req.getHttpVersion(), httpVersion::ONE);
		cr_assert_str_eq(headers.find("Host")->second.c_str(), "test.com");
		cr_assert_str_eq(headers.find("Content-Type")->second.c_str(), "app");
		cr_assert_str_eq(headers.find("Content-Length")->second.c_str(), "18");
		cr_assert_eq(req.getContentLength(), 18);
		cr_assert_str_eq(req.getMessage().c_str(), "this is the body\r\n");
	} catch (std::exception &e) {
		std::cerr << "incorrect format for request\n";
		cr_assert(0);
	}
}

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