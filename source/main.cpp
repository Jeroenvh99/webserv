#include "webserv.hpp"
#include "network/Buffer.hpp"
#include "logging.hpp"
#include "http/Request.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace logging;

int
main() {
	Poller					poller;
	//std::ofstream	logf("log.txt");
	AccessLogger	alog(std::cout, Format{
		Variable("["), Variable(Variable::Type::time_local), Variable("]"),
	});
	ErrorLogger	elog(std::cerr, ErrorLogger::Level::debug);

	auto	abox = poller.add(
		IPv4Acceptor(IPv4Acceptor::Address(1100, INADDR_ANY)),
		{Poller::EventType::read},
		{Poller::Mode::edge_triggered});

	((IPv4Acceptor&)*abox).listen(1000);

	while (true) {
		Poller::Events	events = poller.wait<128>(1000);

		for (auto const& event: events) {
			network::SharedHandle	handle = event.handle();

			if (handle == abox) {
				try {
					IPv4Acceptor const&		acceptor = static_cast<IPv4Acceptor const&>(*handle);
					IPv4Acceptor::Address	address;
					IPv4StreamSocket		client = acceptor.accept(address);
	
					poller.add(std::move(client), dfl_events, dfl_mode);
					elog.log("Connection established", ErrorLogger::Level::notice);
				} catch (std::exception& e) {
					elog.log(e.what(), ErrorLogger::Level::critical);
				}
			} else {
				IPv4StreamSocket const&	client = static_cast<IPv4StreamSocket const&>(*handle);
				network::Buffer<512>	buf;
				http::Request req;

				if (event.happened(network::Poller::EventType::read)) {
					client.read(buf);
					if (buf.len() == 0) { // close host socket automatically?
						poller.remove(handle);
						req.addBuffer(buf);
						elog.log("Connection lost", ErrorLogger::Level::notice);
					} else {
						Client	c;
						alog.log(c);
						std::ostringstream	oss;
						std::cout << buf;
						oss << buf;
						req.addBuffer(buf);
					}
				}
				if (event.happened(network::Poller::EventType::write))
					client.write(buf);
				buf.empty();
			}
		}
	}
}
