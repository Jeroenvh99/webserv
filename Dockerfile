FROM debian:bullseye

RUN apt-get update && apt-get install g++ libcriterion-dev -y

COPY regexengine.cpp regexengine.hpp test/test.cpp include/HttpRequest.hpp source/request/HttpRequest.cpp .
RUN g++ HttpRequest.cpp regexengine.cpp test.cpp -lcriterion

CMD ["./a.out"]