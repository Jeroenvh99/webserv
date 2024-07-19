#include "http/chunk.hpp"
#include <string>
#include <iostream>
#include <exception>


int main() {
	try {
		http::Dechunker msg;
		webserv::Buffer buf;
		buf.push_back("");
		http::Dechunker::Chunk res = msg.dechunk(buf);
		if (res == std::nullopt) {
			std::cout << "All good\n";
		} else {
			throw std::exception();
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	try {
		http::Dechunker msg;
		webserv::Buffer buf;
		buf.push_back("7\r\nMozilla\r\n11\r\nDeveloper Network\r\n0\r\n\r\n");
		http::Dechunker::Chunk res = msg.dechunk(buf);
		if (res == "MozillaDeveloper Network") {
			std::cout << "All good\n";
		} else {
			throw std::exception();
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	try {
		http::Dechunker msg;
		webserv::Buffer buf;
		buf.push_back("7\r\nMozilla\r\n0\r\n\r\n");
		http::Dechunker::Chunk res = msg.dechunk(buf);
		if (res == "Mozilla") {
			std::cout << "All good\n";
		} else {
			throw std::exception();
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	try {
		http::Dechunker msg;
		webserv::Buffer buf;
		buf.push_back("4\r\nMozilla\r\n0\r\n\r\n");
		http::Dechunker::Chunk res = msg.dechunk(buf);
	} catch (http::Dechunker::ChunkException &e) {
		if (e.what() == std::string("bad chunk size")) {
			std::cout << "All good, the correct exception is raised\n";
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	try {
		http::Dechunker msg;
		webserv::Buffer buf;
		buf.push_back("7\r\nMozilla\r\n0\r\n");
		http::Dechunker::Chunk res = msg.dechunk(buf);
	}
	//  catch (http::Dechunker::ChunkException &e) {
	// 	if (e.what() == std::string("bad chunk size")) {
	// 		std::cout << "All good, the correct exception is raised\n";
	// 	}
	// }
	 catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	try {
		http::Dechunker msg;
		webserv::Buffer buf;
		buf.push_back("7\r\nMozilla\r\n0");
		http::Dechunker::Chunk res = msg.dechunk(buf);
	}
	//  catch (http::Dechunker::ChunkException &e) {
	// 	if (e.what() == std::string("bad chunk size")) {
	// 		std::cout << "All good, the correct exception is raised\n";
	// 	}
	// }
	 catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	try {
		http::Dechunker msg;
		webserv::Buffer buf;
		buf.push_back("7\r\nMozilla");
		http::Dechunker::Chunk res = msg.dechunk(buf);
	} catch (http::Dechunker::ChunkException &e) {
		if (e.what() == std::string("bad chunk size")) {
			std::cout << "All good, the correct exception is raised\n";
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
