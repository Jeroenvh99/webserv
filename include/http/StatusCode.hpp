#pragma once

namespace http
{
    enum class StatusCode : int
    {
        OK = 200,
        BadRequest = 400,
        UnAuthorized = 401,
        Forbidden = 403,
        NotFound = 404,
    };
}