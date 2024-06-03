#ifndef ROUTE_HPP
# define ROUTE_HPP

# include "http.hpp"
# include "http/Status.hpp"

# include <forward_list>
# include <filesystem>
# include <unordered_set>
# include <type_traits>

namespace route {
	using Path = std::filesystem::path;
	using PathSegment = Path::iterator;

	class Config {
	public:
		Config(std::string&& = "");

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

		Config&	redirect(Path const&);
		Config&	list_directory() noexcept;
		Config&	forbid_directory() noexcept;
		Config&	set_directory_file(std::string const&);
		Config&	reset_diropts() noexcept;
		Config&	allow_method(http::Method) noexcept;
		// Config&	allow_methods(std::vector<http::Method> const&) noexcept;
		Config&	disallow_method(http::Method) noexcept;
		Config&	reset_methods() noexcept;
		Config&	allow_cgi(std::string const&);
		Config&	disallow_cgi(std::string const&);
		Config&	reset_cgi();

	private:
		friend class Route;

		enum class MethodOption;
		enum class DirectoryOption;
		enum class CGIOption;
		using ExtensionSet = std::unordered_set<std::string>;
		using MethodBitmask = std::underlying_type<http::Method>::type;

		Config(Config const&, std::string const&);
		Config(Config const&, std::string&&);

		Config const*	_super;
		std::string		_fname;
		Path			_redirection;
		MethodOption	_methopt;
		MethodBitmask	_allowed_methods;
		DirectoryOption	_diropt;
		std::string		_directory_file;
		CGIOption		_cgiopt;
		ExtensionSet	_cgi;
	}; // class Config

	using Location = Config;

	class Route: public Config {
	public:
		Route(Path const&);

		Route&		extend(Path const&);
		Location	follow(Path const&) const;
		Route&		seek(Path const&);

	private:
		using Container = std::forward_list<Route>;

		Route(Route const&, std::string const&);
		Route(Route const&, PathSegment, PathSegment);

		Route&		_extend_core(PathSegment, PathSegment);
		Location	_follow_core(PathSegment, PathSegment) const;
		Route&		_seek_core(PathSegment, PathSegment);

		Container::iterator			_subroute(std::string const&) noexcept;
		Container::const_iterator	_subroute(std::string const&) const noexcept;

		Container	_subroutes;
	}; // class Route

	enum class Config::MethodOption {
		inherits,
		own,
	}; // enum class Config::MethodOption

	enum class Config::DirectoryOption {
		inherits,
		forbid,
		listing,
		default_file,
	}; // enum class Config::DirectoryOption

	enum class Config::CGIOption {
		inherits,
		disallow,
		allow,
	}; // enum class Config::CGIOption
}; // namespace route

#endif // ROUTE_HPP
