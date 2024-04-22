#ifndef HTTPRESPONSE_C
# define HTTPRESPONSE_C
# include <vector>
# include <string>
# include <iostream>
# include <sstream>
# include <exception>
# include <algorithm>
# include <unordered_map>

enum httpVersion {
	ONE,
	ONEDOTONE,
	NO
};

class HttpResponse {
	using HeaderMap = std::unordered_map<std::string, std::string>;
	private:
		std::string _buffer;
		httpVersion _version;
		int _statuscode;
		std::string _statustext;
		HeaderMap _headers;
		std::string _message;
	public:
		HttpResponse();
		HttpResponse(const HttpResponse& src);
		std::string getResponse();
		~HttpResponse();
};

#endif