FROM debian:bullseye

RUN apt-get update && apt-get install g++ libcriterion-dev -y

# COPY regexengine.cpp regexengine.hpp test/regextest.cpp test/requesttest.cpp test/configtest.cpp test/chunkertest.cpp source/http/Request.cpp include/Config.hpp source/config/Config.cpp source/http/chunk.cpp .
# COPY test/test1.conf test/test2.conf test/test3.conf test/test4.conf test/test5.conf test/test6.conf .
# COPY include/http/Request.hpp /http/Request.hpp
# COPY include/network/Buffer.hpp /network/Buffer.hpp
# COPY include/http.hpp ./http.hpp
# COPY include/Buffer.hpp ./Buffer.hpp
# COPY include/http/chunk.hpp /http/chunk.hpp
COPY ./* ./
RUN g++ test/regexengine.cpp test/regextest.cpp test/chunkertest.cpp -I$./include/ -lcriterion
# Request.cpp Config.cpp requesttest.cpp configtest.cpp

CMD ["./a.out"]