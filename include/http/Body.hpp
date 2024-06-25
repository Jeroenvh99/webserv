#ifndef HTTP_BODY_HPP
# define HTTP_BODY_HPP

namespace http {
    struct Body {
        enum class Type;

		Body(Type, size_t = 0);

		operator bool() const noexcept;

		Type 	type;
		size_t 	length;
    }; // struct Body

	enum class Body::Type {
		none,
		invalid,
		by_length,
		chunked,
	}; // enum class Body::Type
}; // namespace http

#endif // HTTP_BODY_HPP