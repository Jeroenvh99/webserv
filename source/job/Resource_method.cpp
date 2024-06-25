#include "job/Resource.hpp"

using job::Resource

job::StatusOption

Resource::status() const noexcept {
	return (_status);
}