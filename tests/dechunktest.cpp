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

	// buf.empty();
	// buf.push_back("4\r\nWiki\r\n7\r\npedia i\r\n9\r\nin chunks.\r\n0\r\n\r\n");
	// std::cout << buf << "\n";
	// chunkinator.clear();
	// status = chunkinator.dechunk(buf);
	// std::cout << buf << "\n";
	// if (status == http::Dechunker::Status::done)
	// 	std::cout << "chunks parsed successfully" << "\n";
	// else if (status == http::Dechunker::Status::pending)
	// 	std::cout << "chunk parsing is still pending" << "\n";

	// buf.empty();
	// buf.push_back("4\r\nWiki\r\n7\r\npedia i\r\n9\r\n chunks.\r\n0\r\n\r\n");
	// std::cout << buf << "\n";
	// chunkinator.clear();
	// status = chunkinator.dechunk(buf);
	// std::cout << buf << "\n";
	// if (status == http::Dechunker::Status::done)
	// 	std::cout << "chunks parsed successfully" << "\n";
	// else if (status == http::Dechunker::Status::pending)
	// 	std::cout << "chunk parsing is still pending" << "\n";

	// buf.empty();
	// buf.push_back("4\r\nWiki\r\n7\r\npedia i\r\n9l\r\nn chunks.\r\n0\r\n\r\n");
	// std::cout << buf << "\n";
	// chunkinator.clear();
	// status = chunkinator.dechunk(buf);
	// std::cout << buf << "\n";
	// if (status == http::Dechunker::Status::done)
	// 	std::cout << "chunks parsed successfully" << "\n";
	// else if (status == http::Dechunker::Status::pending)
	// 	std::cout << "chunk parsing is still pending" << "\n";

	// buf.empty();
	// buf.push_back("4\r\nWiki\r\n7\r\npedia i\r\nl9\r\nn chunks.\r\n0\r\n\r\n");
	// std::cout << buf << "\n";
	// chunkinator.clear();
	// status = chunkinator.dechunk(buf);
	// std::cout << buf << "\n";
	// if (status == http::Dechunker::Status::done)
	// 	std::cout << "chunks parsed successfully" << "\n";
	// else if (status == http::Dechunker::Status::pending)
	// 	std::cout << "chunk parsing is still pending" << "\n";

	// buf.empty();
	// buf.push_back("4\r\nWiki\r\n7\r\npedia i\r\n9\r\nn chunks.\r\n0\r\n\r\n9\r\nin chunks.\r\n");
	// std::cout << buf << "\n";
	// chunkinator.clear();
	// status = chunkinator.dechunk(buf);
	// std::cout << buf << "\n";
	// if (status == http::Dechunker::Status::done)
	// 	std::cout << "chunks parsed successfully" << "\n";
	// else if (status == http::Dechunker::Status::pending)
	// 	std::cout << "chunk parsing is still pending" << "\n";

	buf.empty();
	buf.push_back("\r\n");
	std::cout << buf << "\n";
	chunkinator.clear();
	status = chunkinator.dechunk(buf);
	std::cout << buf << "\n";
	if (status == http::Dechunker::Status::done)
		std::cout << "chunks parsed successfully" << "\n";
	else if (status == http::Dechunker::Status::pending)
		std::cout << "chunk parsing is still pending" << "\n";

	buf.empty();
	buf.push_back("4\r\nWiki\r\n7\r\npedia i\r\n9\r\nn chunks.\r\n0\r\n");
	std::cout << buf << "\n";
	chunkinator.clear();
	status = chunkinator.dechunk(buf);
	if (status == http::Dechunker::Status::done)
		std::cout << "chunks parsed successfully" << "\n";
	else if (status == http::Dechunker::Status::pending)
		std::cout << "chunk parsing is still pending" << "\n";
	buf.empty();
	buf.push_back("\r\n");
	status = chunkinator.dechunk(buf);
	std::cout << buf << "\n";
	if (status == http::Dechunker::Status::done)
		std::cout << "chunks parsed successfully" << "\n";
	else if (status == http::Dechunker::Status::pending)
		std::cout << "chunk parsing is still pending" << "\n";
	return 0;
}