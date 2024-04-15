#include "webserv.hpp"
#include "network/Buffer.hpp"
#include "Logger.hpp"

#include <iostream>
#include <string>
#include <sstream>

int
main() {
	try {
		Poller			poller;
		AccessLogger	alog(std::cout);

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
					IPv4Acceptor const&		acceptor = static_cast<IPv4Acceptor const&>(*handle);
					IPv4Acceptor::Address	address;
					IPv4StreamSocket		client = acceptor.accept(address);

					poller.add(std::move(client), dfl_events, dfl_mode);
					alog.log("Connection established");
				} else {
					IPv4StreamSocket const&	client = static_cast<IPv4StreamSocket const&>(*handle);
					network::Buffer<512>	buf;
	
					if (event.happened(network::Poller::EventType::read)) {
						client.read(buf);
						if (buf.len() == 0) { // close host socket automatically?
							poller.remove(handle);
							alog.log("Connection lost");
						} else
							alog.log(std::string(buf));
					}
					if (event.happened(network::Poller::EventType::write))
						client.write(buf);
					buf.empty();
				}
			}
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}
