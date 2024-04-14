#include "CGI.hpp"

void CGI::initEnv()
{
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
}