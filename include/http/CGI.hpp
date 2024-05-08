#pragma once

#include <map>
#include <string>

#include "Request.hpp"

namespace http
{
    class CGI
    {
    public:
        class CGIException : public std::exception
        {
        public:
            std::string message;
            CGIException(const std::string &message) : message("CGI error: " + message){};
            ~CGIException() throw(){};
            virtual const char *what() const throw()
            {
                return message.c_str();
            };
        };

        CGI(const Request &request);

    private:
        using EnvMap = std::map<std::string, std::string>;

        void exec(const Request &request);
        void initEnv(const Request &request);

        EnvMap _env;
    };
}