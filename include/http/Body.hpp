#ifndef HTTP_BODY_HPP
# define HTTP_BODY_HPP

# include <cstddef>

namespace http {
    class Body {
	public:
        enum class Type {
			none,
			by_length,
			chunked,
		}; // enum class Type

		Body(Type = Type::none, size_t = 0);

		operator bool() const noexcept;

		Type 	type() const noexcept;
		size_t 	length() const noexcept;
	private:
		Type	_type;
		size_t	_length;
    }; // class Body

	struct BodyPart {
		BodyPart():
			is_last(false) {};

		Headers		headers;
		std::string	body;
		bool		is_last;
	}; // struct BodyPart
}; // namespace http

#endif // HTTP_BODY_HPP