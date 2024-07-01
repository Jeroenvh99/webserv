#include "job/Resource.hpp"

using job::Resource;

job::Status
Resource::status() const noexcept {
	return (_status);
}