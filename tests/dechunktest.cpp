#include "http/chunk.hpp"
#include "utils/utils.hpp"

#include <string>
#include <iostream>

int main() {
	webserv::Buffer buf;
	buf.push_back("4\r\nWiki\r\n7\r\npedia i\r\n9\r\nn chunks.\r\n0\r\n\r\n");
	std::cout << buf << "\n";
	http::Dechunker chunkinator;
	http::Dechunker::Status status = chunkinator.dechunk(buf);
	std::cout << buf << "\n";
	if (status == http::Dechunker::Status::done)
		std::cout << "chunks parsed successfully" << "\n";
	else if (status == http::Dechunker::Status::pending)
		std::cout << "chunk parsing is still pending" << "\n";
	return 0;
}