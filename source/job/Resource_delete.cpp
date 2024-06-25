#include "job/Resource.hpp"

job::StatusOption
Resource::_delete(std::string&, stdfs::path const& pt) {
	if (!std::filesystem::remove(pt))
		return ({true, http::Status::internal_error});
	return ({true, http::Status::no_content});
}