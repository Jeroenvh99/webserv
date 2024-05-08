FROM debian:bullseye

RUN apt-get update && apt-get install g++ libcriterion-dev -y

COPY regexengine.cpp regexengine.hpp test/regextest.cpp test/requesttest.cpp test/configtest.cpp include/HttpRequest.hpp source/request/HttpRequest.cpp include/Config.hpp source/config/Config.cpp .
RUN g++ HttpRequest.cpp Config.cpp regexengine.cpp regextest.cpp requesttest.cpp configtest.cpp -lcriterion

CMD ["./a.out"]