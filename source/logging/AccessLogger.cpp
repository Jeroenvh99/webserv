#include "logging.hpp"

using logging::AccessLogger;
using logging::Variable;

AccessLogger::AccessLogger(std::ostream& os, Format const& fmt):
	Logger(os),
	_fmt(fmt) {}

AccessLogger::AccessLogger(std::ostream& os, Format&& fmt):
	Logger(os),
	_fmt(fmt) {}

void
AccessLogger::log(Client const&/* client*/) {
	timestamp_update();
	for (Variable const& elem: _fmt) {
		switch (elem.type()) {
		case Variable::Type::literal:
			os() << elem.data();
			break;
		case Variable::Type::request:
			//os() << client.request();
			break;
		case Variable::Type::status:
			//os() << client.status();
			break;
		case Variable::Type::time_local:
			os() << timestamp();
			break;
		default: break; // unreachable
		}
	}
	os() << std::endl;
}
