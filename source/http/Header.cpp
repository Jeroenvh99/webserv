#include "http/Header.hpp"
#include "http/parse.hpp"

#include <sstream>

using http::Header;
using http::Headers;

/* class Header */

// Basic operations

// template<typename T, typename>
// Header::Header(Header::Impl& impl):
// 	_impl(impl) {}

Header::Header(Header::Impl const& impl):
	_impl(const_cast<Header::Impl&>(impl)) {} // Trust me on this.

// Conversions

Header::operator std::string() const {
	return (name() + ": " + csvalue());
}

// Accessors

Header::Key const&
Header::name() const noexcept {
	return (_impl.first);
}

Header::Value const&
Header::value() const noexcept {
	return (_impl.second);
}

Header::Value&
Header::value() noexcept {
	return (_impl.second);
}

std::string
Header::csvalue() const {
	std::ostringstream	oss;
	auto				it = value().begin();

	for (; std::next(it, 1) != value().end(); std::advance(it, 1))
		oss << *it << ',';
	oss << *it;
	return (oss.str());
}

/* class Headers */

// Accessors

Header const
Headers::at(Header::Key const& name) const {
	auto const	it = find(name);

	if (it == end())
		throw (std::out_of_range(""));
	return {*it};
}

Header
Headers::at(Header::Key const& name) {
	auto const	it = find(name);

	if (it == end())
		throw (std::out_of_range(""));
	return {*it};
}

bool
Headers::contains(Header::Key const& name, Header::Value::value_type const& val) const {
	auto const	it = find(name);

	if (it == end())
		return (false);
	return (it->second.contains(val));
}

// Modifiers

Headers::iterator
Headers::insert(std::string const& str) {
	value_type	h(to_header(str));

	return (insert_or_assign(std::move(h.first), std::move(h.second)).first);
}

Headers::iterator
Headers::update(std::string const& str) {
	return (update(to_header(str)));
}

Headers::iterator
Headers::update(Headers::value_type const& hdr) {
	auto	it = find(hdr.first);

	if (it == end())
		return (insert(hdr).first);
	for (auto const& sval: hdr.second)
		it->second.insert(sval);
	return (it);
}

Headers::iterator
Headers::update(Headers::value_type&& hdr) {
	auto	it = find(hdr.first);

	if (it == end())
		return (insert(std::move(hdr)).first);
	for (auto&& sval: hdr.second)
		it->second.insert(sval);
	return (it);
}

Header::Impl
Headers::to_header(std::string const& str) {
	std::istringstream	iss(str);
	Header::Key			name;
	Header::Value		value;
	
	std::getline(iss, name, ':');
	if (name.length() == 0)
		throw (parse::HeaderException("nameless header is invalid"));
	while (iss) {
		std::string	sval;

		std::getline(iss, sval, ',');
		value.insert(std::move(sval));
	}
	return {name, value};
}

// Non-member functions

std::ostream&
http::operator<<(std::ostream& os, Headers const& that) {
	for (auto const& himpl: that)
		os << std::string(Header(himpl)) << "\r\n";
	return (os);
}