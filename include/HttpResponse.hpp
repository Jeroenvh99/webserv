#ifndef HTTPRESPONSE_C
# define HTTPRESPONSE_C
# include <vector>
# include <string>
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
		std::string _response;
		httpVersion _version;
		int _statuscode;
		std::string _statustext;
		HeaderMap _headers;
		std::string _message;
	public:
		HttpResponse(httpVersion version, int &status, HeaderMap &headers, std::string &message);
		HttpResponse(const HttpResponse& src);
		HttpResponse &operator=(const HttpResponse& src);
		std::string getResponse();
		httpVersion getHttpVersion() const;
		int getStatusCode() const;
		std::string getStatusText() const;
		HttpResponse::HeaderMap getHeaders() const;
		std::string getMessage() const;
		~HttpResponse();
		class InvalidResponseException: public std::exception {
			virtual const char* what() const throw();
		};
};

#endif