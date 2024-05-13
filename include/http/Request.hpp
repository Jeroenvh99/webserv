#pragma once

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
		GET,
		HEAD,
		POST,
		PUT,
		DELETE,
		CONNECT,
		OPTIONS,
		TRACE,
		NONE
	};

	enum HttpVersion
	{
		ONE,
		ONEDOTONE,
		NO
	};

	class Request
	{
		using HeaderMap = std::unordered_multimap<std::string, std::string>;

	private:
		std::string _buffer;
		RequestMethod _method;
		HttpVersion _version;
		std::string _requesturi;
		HeaderMap _headers;
		std::string _message;
		int _contentlength;
		bool _hasbody;

	public:
		Request(int contentlength = -1);
		Request(const Request &src);
		Request &operator=(const Request &src);
		~Request();

		void addBuffer(std::array<char, 512> request);
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