#ifndef URI_HPP
# define URI_HPP

# include <map>
# include <string>
# include <vector>

class URI {
private:
	std::vector<std::string>			_path;
	std::vector<std::string>			_pathinfo;
	std::map<std::string, std::string>	_query;
	std::string							_fragment;
}; // class URI

#endif // URI_HPP
