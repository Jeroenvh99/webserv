#pragma once

#include <map>
#include <string>

#include "../HttpRequest.hpp"

using EnvMap = std::map<std::string, std::string>;

class CGI
{
    public:
		class CGIException: public std::exception {
            public:
                std::string message;
                CGIException(const std::string& message) : message("CGI error: " + message){};
                ~CGIException() throw(){};
                virtual const char* what() const throw() {
                    return message.c_str();
                };
		};

        CGI(const HttpRequest& request);
    private:
        void exec(const HttpRequest &request);
        void initEnv(const HttpRequest& request);

        EnvMap _env;
};