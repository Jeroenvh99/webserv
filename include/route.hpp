#ifndef ROUTE_HPP
# define ROUTE_HPP

# include "http.hpp"
# include "http/Status.hpp"

# include <forward_list>
# include <filesystem>
# include <unordered_set>
# include <type_traits>

namespace route {
	class BaseRoute;
	class Route;
	class Location;
	using Path = std::filesystem::path;

	class BaseRoute {
	public:
		BaseRoute(bool = false);
		BaseRoute(Route const&);

		bool				lists_directory() const noexcept;
		bool				forbids_directory() const noexcept;
		std::string const&	directory_file() const noexcept;
		bool				allows_method(http::Method) const noexcept;
		bool				allows_cgi(std::string const&) const noexcept;

		static constexpr char const*	no_directory_file = "";

	private:
		friend class Route;
		friend class Location;

		enum class MethodOption;
		enum class DirectoryOption;
		enum class CGIOption;
		using ExtensionSet = std::unordered_set<std::string>;
		using MethodBitmask = std::underlying_type<http::Method>::type;

		MethodOption	_methopt;
		MethodBitmask	_allowed_methods;
		DirectoryOption	_diropt;
		std::string		_directory_file;
		CGIOption		_cgiopt;
		ExtensionSet	_cgi;
	}; // class BaseRoute

	class Route: public BaseRoute {
	public:
		Route(Path const& = "");

		Path		from() const;
		Path		to() const;
		Route&		extend(Path const&);
		Location	follow(Path const&) const;
		Route&		seek(Path const&);

		bool				lists_directory() const noexcept;
		bool				forbids_directory() const noexcept;
		std::string const&	directory_file() const noexcept;
		bool				allows_method(http::Method) const noexcept;
		bool				allows_cgi(std::string const&) const noexcept;
		std::string const&	filename() const noexcept;

		Route&	redirect(Path const&);
		Route&	list_directory() noexcept;
		Route&	forbid_directory() noexcept;
		Route&	set_directory_file(std::string const&);
		Route&	reset_diropts() noexcept;
		Route&	allow_method(http::Method) noexcept;
		// Route&	allow_methods(std::vector<http::Method> const&) noexcept;
		Route&	disallow_method(http::Method) noexcept;
		Route&	reset_methods() noexcept;
		Route&	allow_cgi(std::string const&);
		Route&	disallow_cgi(std::string const&);
		Route&	reset_cgi();

		static Path const	no_redirection;

	private:
		friend class BaseRoute;

		using Container = std::forward_list<Route>;

		Route(Route const&, std::string const&);
		Route(Route const&, std::string&&);
		Route(Route const&, Path::iterator, Path::iterator);

		Route&		_extend_core(Path::iterator, Path::iterator);
		Location	_follow_core(Path::iterator, Path::iterator) const;
		Route&		_seek_core(Path::iterator, Path::iterator);

		MethodBitmask		_super_allowed_methods() const noexcept;
		DirectoryOption		_super_diropt() const noexcept;
		std::string const&	_super_directory_file() const noexcept;
		CGIOption			_super_cgiopt() const noexcept;
		ExtensionSet		_super_cgi() const noexcept;

		Container::iterator			_subroute(std::string const&) noexcept;
		Container::const_iterator	_subroute(std::string const&) const noexcept;

		Route const*	_super;
		Container		_subroutes;
		std::string		_fname;
		Path			_redirection;
	}; // class Route

	class Location: public BaseRoute {
	public:
		Location(Route const&, Path const& = "");

		Path const&			from() const noexcept;
		Path const&			to() const noexcept;
		std::string const&	path_info() const noexcept;

	private:
		Path		_from;
		Path		_to;
		std::string	_path_info;
	}; // class Location

	enum class BaseRoute::MethodOption {
		inherits,
		own,
	}; // enum class BaseRoute::MethodOption

	enum class BaseRoute::DirectoryOption {
		inherits,
		forbid,
		listing,
		default_file,
	}; // enum class BaseRoute::DirectoryOption

	enum class BaseRoute::CGIOption {
		inherits,
		disallow,
		allow,
	}; // enum class BaseRoute::CGIOption
}; // namespace route

#endif // ROUTE_HPP
