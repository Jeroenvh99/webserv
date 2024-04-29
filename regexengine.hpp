#ifndef REGEXENGINE_H
# define REGEXENGINE_H
# include <vector>
# include <string>

namespace regex_utils {
	enum pattern_state {
		NORMAL,
		BACKSLASH,
		SQUAREBRACKETS,
		NONE
	};
}

int regexsearch(std::vector<std::string> info);

#endif