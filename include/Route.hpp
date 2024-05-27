#ifndef ROUTE_HPP
# define ROUTE_HPP

# include "http.hpp"
# include "http/Status.hpp"

# include <filesystem>
# include <unordered_set>
# include <type_traits>
# include <vector>

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

	void	redirect(Path const&);
	void	list_directory() noexcept;
	void	forbid_directory() noexcept;
	void	set_directory_file() noexcept;
	void	reset_diropts() noexcept;
	void	allow_method(http::Method) noexcept;
	void	disallow_method(http::Method) noexcept;
	void	reset_methods() noexcept;
	void	allow_cgi(std::string const&);
	void	disallow_cgi(std::string const&);
	void	reset_cgi();

private:
	friend class Route;

	enum class MethodOption;
	enum class DirectoryOption;
	enum class CGIOption;
	using ExtensionSet = std::unordered_set<std::string>;
	using MethodBitmask = std::underlying_type<http::Method>::type;

	RouteConfig(RouteConfig const&, std::string&&);

	RouteConfig const*	_super;
	std::string		_fname;
	Path			_redirection;
	MethodOption	_methopt;
	MethodBitmask	_allowed_methods;
	DirectoryOption	_diropt;
	std::string		_directory_file;
	CGIOption		_cgiopt;
	ExtensionSet	_cgi;
}; // class RouteConfig

class Route: public RouteConfig {
public:
	Route(Path const&);

	void		extend(Path const&);
	RouteConfig	follow(Path const&);

private:
	Route(Route const&, PathSegment, PathSegment);

	void		_extend_core(PathSegment, PathSegment);
	RouteConfig	_follow_core(PathSegment, PathSegment);

	std::vector<Route>	_subroutes;
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
