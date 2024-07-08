#ifndef HTTP_BODY_HPP
# define HTTP_BODY_HPP

# include <cstddef>

namespace http {
    class Body {
	public:
        enum class Type {
			none,
			by_length,
			to_dechunk,
			to_enchunk,		
		}; // enum class Type

		Body(Type = Type::none, size_t = 0);

		operator bool() const noexcept;

		Type 	type() const noexcept;
		size_t 	length() const noexcept;
	private:
		Type	_type;
		size_t	_length;
    }; // class Body
}; // namespace http

#endif // HTTP_BODY_HPP