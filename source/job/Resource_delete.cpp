#include "job/Resource.hpp"

using job::Resource;

http::Status
Resource::_delete(route::Location const& loc) {
	stdfs::path const&	pt = loc.to();

	if (!std::filesystem::remove(pt)) {
		_status = Status::failure;
		return (http::Status::internal_error);
	}
	_status = Status::success;
	return (http::Status::no_content);
}