FROM debian:bullseye

RUN apt-get update && apt-get install g++ libcriterion-dev -y

COPY HttpHeader.cpp regexengine.cpp regexengine.hpp test.cpp .
RUN g++ HttpHeader.cpp regexengine.cpp test.cpp -lcriterion

CMD ["./a.out"]