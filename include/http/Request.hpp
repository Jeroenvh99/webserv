#pragma once

#include "Server.hpp"

#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
#include <array>
#include <unordered_map>

namespace http
{
	enum RequestMethod
	{
		NONE = 0x0,
		GET = 0x1 < 0,
		HEAD = 0x1 < 1,
		POST = 0x1 < 2,
		PUT = 0x1 < 3,
		DELETE = 0x1 < 4,
		CONNECT = 0x1 < 5,
		OPTIONS = 0x1 < 6,
		TRACE = 0x1 < 7,
	};

	enum HttpVersion
	{
		ONE,
		ONEDOTONE,
		NO
	};

	class Request
	{
		using HeaderMap = std::unordered_map<std::string, std::string>;

	private:
		std::string _buffer;
		RequestMethod _method;
		HttpVersion _version;
		std::string _requesturi;
		HeaderMap _headers;
		std::string _message;
		int _contentlength;

	public:
		Request(int contentlength = -1);
		Request(const Request &src);
		Request &operator=(const Request &src);
		~Request();

		void addBuffer(Server::Buffer const& src);
		void parse(std::string &request);
		void parseRequestLine(std::stringstream &s);
		void parseHeaders(std::stringstream &s);
		void parseBody(std::stringstream &s);
		static bool isHttpHeader(std::string &header);
		const RequestMethod &getMethod() const;
		const HttpVersion &getHttpVersion() const;
		const std::string &getRequestUri() const;
		const HeaderMap &getHeaders() const;
		const std::string &getMessage() const;
		const int &getContentLength() const;

		class IncorrectRequestFormatException : public std::exception
		{
			virtual const char *what() const throw();
		};
		class IncorrectHeaderFormatException : public std::exception
		{
			virtual const char *what() const throw();
		};
	};
}
