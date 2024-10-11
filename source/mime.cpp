#include "mime.hpp"

char const*
mime::get_type(std::filesystem::path const& pt) {
	if (!pt.has_extension())
		return (default_type);
	try {
		return (types.at(pt.extension().c_str()));
	} catch (std::out_of_range&) {
		return (default_type);
	}
}