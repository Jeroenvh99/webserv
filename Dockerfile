FROM debian:bullseye

RUN apt-get update && apt-get install g++ libcriterion-dev -y

COPY regexengine.cpp regexengine.hpp test/regextest.cpp test/requesttest.cpp test/configtest.cpp source/http/Request_Parser/* source/config/Config.cpp .
COPY test/test1.conf test/test2.conf test/test3.conf test/test4.conf test/test5.conf test/test6.conf test/test7.conf .
COPY include/* ./
COPY include/http/* ./http/
COPY include/logging/* ./logging/
COPY include/network/* ./network/
# RUN g++ Parser.cpp Config.cpp regexengine.cpp regextest.cpp requesttest.cpp configtest.cpp -lcriterion

CMD ["ls -R"]