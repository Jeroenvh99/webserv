FROM debian:bullseye

RUN apt-get update && apt-get install g++ libcriterion-dev -y

COPY ParseBody.cpp HttpHeader.cpp ParseRequestLine.cpp regexengine.cpp regexengine.hpp test.cpp HttpRequest.hpp .
RUN g++ ParseBody.cpp HttpHeader.cpp ParseRequestLine.cpp regexengine.cpp test.cpp -lcriterion

CMD ["./a.out"]