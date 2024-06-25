#include "job/Resource.hpp"
#include "html.hpp"

Resource::Resource():
	_type(Type::none) {}

Resource::~Resource() {
	close();
}

Resource::Resource(Resource&& that):
	_type(that._type) {
	switch (_type) {
	case Type::file:
		new (&_file) std::fstream(std::move(that._file));
		break;
	case Type::builtin:
		new (&_builtin) std::stringstream(std::move(that._builtin));
		break;
	case Type::none: break;
	}
}

Resource::Resource(Job const& job):
	_type(Type::none) {
	switch (job.request.method()) {
	case http::Method::GET:
		_status = _get(job.location.to());
		break;
	case http::Method::POST:
		_status = _post(job.location.to());
		break;
	case http::Method::DELETE:
		_status = _delete(job.location.to());
		break;
	default:
		_status = http::Status::not_implemented;
	}
}

Resource::Resource(ErrorJob const& job):
	_type(Type::none) {
	job::StatusOption	status = _get_file(job.file));

	if (status && *status == http::Status::not_found) { // nonexistent errpage
		close();
		_open_builtin(default_html(job.status));
		_status = job.status; // just for good measure
	}
}

Resource&
Resource::operator=(Resource&& that) {
	if (this == &that)
		return (*this);
	close();
	_type = that._type;
	switch (_type) {
	case Type::file:
		new (&_file) std::fstream(std::move(that._file));
		break;
	case Type::builtin:
		new (&_builtin) std::stringstream(std::move(that._builtin));
		break;
	case Type::none: break;
	}
	return (*this);
}