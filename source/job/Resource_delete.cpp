#include "job/resource.hpp"

using job::Resource;

http::Status
Resource::_delete(route::Location const& loc) {
	stdfs::path const&	pt = loc.to();

	if (!std::filesystem::remove(pt))
		return (http::Status::internal_error);
	return (http::Status::no_content);
}