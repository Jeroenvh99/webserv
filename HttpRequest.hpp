#ifndef HTTPREQUEST_C
# define HTTPREQUEST_C
# include <vector>
# include <string>
# include <iostream>
# include <sstream>
# include <exception>
# include <algorithm>
# include <unordered_map>

enum requestType {
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

enum httpVersion {
	ONE,
	ONEDOTONE,
	NONE
};

enum RequestParseStatus {
	REQUESTLINE,
	HEADERS,
	BODY,
	END
};

class HttpRequest {
	using HeaderMap = std::unordered_map<std::string, std::string>;
	private:
		std::string _buffer;
		requestType _method;
		httpVersion _version;
		std::string _requesturi;
		HeaderMap _headers;
		std::string _message;
		int _contentlength;
	public:
		HttpRequest(char *request);
		HttpRequest(const HttpRequest& src);
		void parse(std::string& request);
		bool isHttpHeader(std::string& header);
		~HttpRequest();
		class IncorrectRequestFormatException: public std::exception {
			virtual const char* what() const throw();
		};
		class IncorrectHeaderFormatException: public std::exception {
			virtual const char* what() const throw();
		};
};

#endif