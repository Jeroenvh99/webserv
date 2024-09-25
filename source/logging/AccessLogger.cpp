#include "logging/logging.hpp"

using logging::AccessLogger;
using logging::Variable;

AccessLogger::AccessLogger():
	Logger(std::cout),
	_fmt(default_fmt) {}

void
AccessLogger::log(Client const&/* client*/, VirtualServer const& server) {
	using Type = Variable::Type;

	std::ostream&	out = os();

	timestamp_update();
	for (Variable const& elem: _fmt) {
		switch (elem.type()) {
		case Type::literal:
			out << elem.data();
			break;
		case Type::request:
			//out << client.request();
			break;
		case Type::status:
			//out << client.status();
			break;
		case Type::time_local:
			out << timestamp();
			break;
		case Type::host:
			out << server.name() << ":" server.port();
			break;
		case Type::client:
			out << client.address();
			break;
		default:
			__builtin_unreachable();
		}
	}
	out << std::endl;
}
