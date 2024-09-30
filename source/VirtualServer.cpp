#include "Server.hpp"

// Basic operations

VirtualServer::VirtualServer(Config::Server config) :
_name(config.servername),
_port(config.port),
_route("/"),
_maxbodysize(config.maxbodysize) {
	for (size_t i = 0; i < config.redirections.size(); i++) {
		add_httpredirect(config.redirections.at(i).from, config.redirections.at(i).to, config.redirections.at(i).permanent);
	}
	for (Config::Location loc : config.locations) {
		for (size_t i = 0; i < loc.paths.size(); i++) {
			_route.extend(loc.paths.at(i))
				.redirect(loc.root)
				.list_directory();
			if (!loc.index.empty()) {
				_route.seek(loc.paths.at(i)).set_directory_file(loc.index);
			}
			for (size_t j = 0; j < loc.allowedmethods.size(); j++) {
				if (loc.allowedmethods.at(j) != http::Method::NONE) {
					_route.seek(loc.paths.at(i)).allow_method(loc.allowedmethods.at(j));
				} else {
					_route.seek(loc.paths.at(i)).disallow_method(loc.allowedmethods.at(j));
				}
			}
			for (size_t j = 0; j < loc.allowedcgi.size(); j++) {
				_route.seek(loc.paths.at(i)).allow_cgi(loc.allowedcgi[j]);
			}
		}
	}
	for (auto& errorpage : config.errorpages) {
		_error_pages.insert(std::pair<http::Status, std::filesystem::path>(static_cast<http::Status>(errorpage.first), errorpage.second));
	}
}

// Accessors

std::string const&
VirtualServer::name() const noexcept {
	return (_name);
}

int const&
VirtualServer::port() const noexcept {
	return (_port);
}

route::Route const&
VirtualServer::route() const noexcept {
	return (_route);
}

route::Location
VirtualServer::locate(std::filesystem::path const& path) const {
	return (_route.follow(path));
}

route::Location
VirtualServer::locate(URI const& uri) const {
	return (locate(uri.path()));
}

stdfs::path const&
VirtualServer::locate_errpage(http::Status status) const noexcept {
	auto const	it = _error_pages.find(status);

	if (it == _error_pages.end())
		return (no_errpage);
	return (it->second);
}

size_t
VirtualServer::max_body_size() const noexcept {
	return (_maxbodysize);
}

VirtualServer::Redirections const&
VirtualServer::redirections() const noexcept {
	return (_redirections);
}

// Modifiers

void
VirtualServer::add_httpredirect(std::string const& from, std::string const& to, bool permanent) {
	_redirections.emplace_back(
		VirtualServer::Redirection{URI(from), URI(to), permanent}
	);
}