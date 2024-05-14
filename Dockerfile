FROM debian:bullseye

RUN apt-get update && apt-get install g++ libcriterion-dev -y

COPY regexengine.cpp regexengine.hpp test/regextest.cpp test/requesttest.cpp test/configtest.cpp source/http/Request.cpp include/Config.hpp source/config/Config.cpp .
COPY test/test1.conf test/test2.conf test/test3.conf test/test4.conf .
COPY include/http/Request.hpp /http/Request.hpp
RUN g++ Request.cpp Config.cpp regexengine.cpp regextest.cpp requesttest.cpp configtest.cpp -lcriterion

CMD ["./a.out"]