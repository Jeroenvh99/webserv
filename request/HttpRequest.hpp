#ifndef HTTPREQUEST_C
# define HTTPREQUEST_C
# include <string>
# include <sstream>
# include <exception>
# include <algorithm>
# include <array>
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
	NO
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
		HttpRequest();
		HttpRequest(const HttpRequest& src);
		void addBuffer(std::array<char, 512> request);
		void parse(std::string& request);
		bool isHttpHeader(std::string& header);
		const requestType &getRequestType();
		const httpVersion &getHttpVersion();
		const std::string &getRequestUri();
		const HeaderMap &getHeaders();
		const std::string &getMessage();
		~HttpRequest();
		class IncorrectRequestFormatException: public std::exception {
			virtual const char* what() const throw();
		};
		class IncorrectHeaderFormatException: public std::exception {
			virtual const char* what() const throw();
		};
};

#endif