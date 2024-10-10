#ifndef MIME_HPP
# define MIME_HPP

# include <filesystem>
# include <string>
# include <unordered_map>

namespace mime {

char const*	get_type(std::filesystem::path const&);

inline const std::unordered_map<std::string, char const*>	types = {
	{".json", "application/json"},
	{".pdf", "application/pdf"},
	{".xml", "application/xml"},
	{".zip", "application/zip"},
	{".jpg", "image/jpeg"},
	{".jpeg", "image/jpeg"},
	{".png", "image/png"},
	{".svg", "image/svg+xml"},
	{".ico", "image/vnd.microsoft.icon"},
	{".css", "text/css"},
	{".txt", "text/plain"},
	{".html", "text/html"},
	{".htm", "text/html"},
	{".js", "text/javascript"},
};

inline char const*	default_type = "application/octet_stream";

}; // namespace mime

#endif // MIME_HPP