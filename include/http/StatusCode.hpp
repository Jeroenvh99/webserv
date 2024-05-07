#ifndef HTTP_STATUS_CODE_HPP
# define HTTP_STATUS_CODE_HPP

namespace http
{
    enum class StatusCode : int
    {
        OK = 200,
        BadRequest = 400,
        Unauthorized = 401,
        Forbidden = 403,
        NotFound = 404,
    };
}

#endif // HTTP_STATUS_CODE_HPP
