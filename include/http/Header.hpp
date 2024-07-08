#ifndef HTTP_HEADER_HPP
# define HTTP_HEADER_HPP

# include "http.hpp"

# include <iostream>
# include <string>
# include <unordered_map>
# include <unordered_set>
# include <utility>

namespace http {
	struct cmp {
		bool	operator()(std::string const& a, std::string const& b) const noexcept {
			return (http::strcmp_nocase(a, b));
		}
	}; // struct cmp

	class Header {
	public:
		using Key = std::string;
		using Value = std::unordered_set<std::string, std::hash<std::string>, cmp>;
		using Impl = std::pair<Key const, Value>;

		template<typename T = Header, typename = std::enable_if_t<!std::is_const<T>::value>>
		Header(Impl& impl): _impl(impl) {}
		Header(Impl const&);

		operator std::string() const;

		Key const&		name() const noexcept;
		Value const&	value() const noexcept;
		Value&			value() noexcept;
		std::string		csvalue() const;

	private:
		Impl&	_impl;
	}; // class Header

	class Headers: public std::unordered_map<
		Header::Key, Header::Value, std::hash<Header::Key>, cmp> {
	public:
		using Base = std::unordered_map<
			Header::Key, Header::Value, std::hash<Header::Key>, cmp>;

		static Header::Impl	parse(std::string const&);

		using Base::insert;
		iterator	insert(std::string const&);
		iterator	update(std::string const&);
		iterator	update(value_type const&);
		iterator	update(value_type&&);

		using Base::at;
		Header const	at(Header::Key const&) const;
		Header			at(Header::Key const&);
		using Base::contains;
		bool			contains(Header::Key const&, Header::Value::value_type const&) const;

	private:
	}; // class Headers

	std::ostream&	operator<<(std::ostream&, Header const&);
	std::ostream&	operator<<(std::ostream&, Headers const&);
}; // namespace http

#endif // HTTP_HEADER_HPP