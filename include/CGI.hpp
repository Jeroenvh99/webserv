#pragma once

#include <map>
#include <string>

using EnvMap = std::map<std::string, std::string>;

class CGI
{
    private:
        void initEnv();

        EnvMap _env;
};