#ifndef ROUTE_HPP
# define ROUTE_HPP

# include "http.hpp"
# include "http/Status.hpp"

# include <forward_list>
# include <filesystem>
# include <unordered_set>
# include <type_traits>

using Path = std::filesystem::path;
using PathSegment = Path::iterator;

class RouteConfig {
public:
	RouteConfig(std::string&& = "");

	static const Path				no_redirection;
	static constexpr char const*	no_directory_file = "";

	std::string const&	filename() const noexcept;
	Path				from() const;
	Path				to() const;

	bool				lists_directory() const noexcept;
	bool				forbids_directory() const noexcept;
	std::string const&	directory_file() const noexcept;
	bool				allows_method(http::Method) const noexcept;
	bool				allows_cgi(std::string const&) const noexcept;

	RouteConfig&	redirect(Path const&);
	RouteConfig&	list_directory() noexcept;
	RouteConfig&	forbid_directory() noexcept;
	RouteConfig&	set_directory_file(std::string const&);
	RouteConfig&	reset_diropts() noexcept;
	RouteConfig&	allow_method(http::Method) noexcept;
	RouteConfig&	disallow_method(http::Method) noexcept;
	RouteConfig&	reset_methods() noexcept;
	RouteConfig&	allow_cgi(std::string const&);
	RouteConfig&	disallow_cgi(std::string const&);
	RouteConfig&	reset_cgi();

private:
	friend class Route;

	enum class MethodOption;
	enum class DirectoryOption;
	enum class CGIOption;
	using ExtensionSet = std::unordered_set<std::string>;
	using MethodBitmask = std::underlying_type<http::Method>::type;

	RouteConfig(RouteConfig const&, std::string const&);
	RouteConfig(RouteConfig const&, std::string&&);

	RouteConfig const*	_super;
	std::string			_fname;
	Path				_redirection;
	MethodOption		_methopt;
	MethodBitmask		_allowed_methods;
	DirectoryOption		_diropt;
	std::string			_directory_file;
	CGIOption			_cgiopt;
	ExtensionSet		_cgi;
}; // class RouteConfig

class Route: public RouteConfig {
public:
	using SubrouteCtr = std::forward_list<Route>;
	using SubrouteIt = SubrouteCtr::iterator;

	Route(Path const&);

	Route&		extend(Path const&);
	RouteConfig	follow(Path const&);
	Route&		seek(Path const&);

private:
	Route(Route const&, std::string const&);
	Route(Route const&, PathSegment, PathSegment);

	Route&		_extend_core(PathSegment, PathSegment);
	RouteConfig	_follow_core(PathSegment, PathSegment);
	Route&		_seek_core(PathSegment, PathSegment);

	std::forward_list<Route>	_subroutes;
}; // class Route

enum class RouteConfig::MethodOption {
	inherits,
	own,
}; // enum class Route::Options::DirectoryOption

enum class RouteConfig::DirectoryOption {
	inherits,
	forbid,
	listing,
	default_file,
}; // enum class Route::Options::DirectoryOption

enum class RouteConfig::CGIOption {
	inherits,
	disallow,
	allow,
}; // enum class Route::Options::DirectoryOption

#endif // ROUTE_HPP
