#ifndef URI_HPP
# define URI_HPP

# include <filesystem>
# include <string>

class URI {
public:
	URI() = default;
	URI(std::string const&);
	operator std::string() const;

	std::filesystem::path const&	path() const noexcept;
	std::filesystem::path const&	path_info() const noexcept;
	std::string const&				query() const noexcept;
	std::string const&				fragment() const noexcept;

	static constexpr char	query_sign = '?';
	static constexpr char	fragment_sign = '#';

private:
	URI(std::string::const_iterator,
		std::string::const_iterator,
		std::string::const_iterator,
		std::string::const_iterator);
	URI(std::filesystem::path const&, std::string&&, std::string&&);

	std::filesystem::path		_path;
	std::filesystem::path		_path_info;
	std::string					_query;
	std::string					_fragment;
}; // class URI

#endif // URI_HPP
