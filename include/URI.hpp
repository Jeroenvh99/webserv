#ifndef URI_HPP
# define URI_HPP

# include <filesystem>
# include <string>

class URI {
public:
	URI(std::string const&);
	URI(std::string::const_iterator,
		std::string::const_iterator,
		std::string::const_iterator,
		std::string::const_iterator);

	std::filesystem::path const&	path() const noexcept;
	std::string const&				query() const noexcept;
	std::string const&				fragment() const noexcept;
	
private:
	std::filesystem::path		_path;
	std::string					_query;
	std::string					_fragment;
}; // class URI

#endif // URI_HPP
