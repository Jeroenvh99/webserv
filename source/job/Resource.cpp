#include "job/Resource.hpp"

using job::Resource;

Resource::Resource():
	_status(Status::pending) {}

job::Status
Resource::status() const noexcept {
	return (_status);
}