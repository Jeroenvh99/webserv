#include "http/StatusCode.hpp"

using http::StatusCode;

// Basic operations
StatusCode::StatusCode(StatusCode::Code code):
	_code(code) {}

// Conversions

StatusCode::operator std::string() const noexcept {
	return (std::to_string(static_cast<int>(_code)));
}

// Public methods

char const*
StatusCode::description() const noexcept {
	for (auto const& [key, desc]: _descriptions)
		if (_code == key)
			return (desc);
	return (_no_description);
}
