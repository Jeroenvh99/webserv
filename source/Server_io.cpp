#include "Server.hpp"
#include "http/Response.hpp"
#include "logging/logging.hpp"

using Elog = logging::ErrorLogger::Level;

static void	validate_body_size(Client const&, size_t);

Server::IOStatus
Server::_parse_request(Client& client) {
	webserv::Buffer	buf;

	if (_recv(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	logging::elog.log(Elog::debug, client.address(),
		": Directing ", buf.len(), " bytes to request parser.");
	try {
		if (client.parse_request(buf)) {
			VirtualServer const&	vserv = virtual_server(client);

			validate_body_size(client, vserv.max_body_size());
			client.respond({client, *this, vserv});
			logging::elog.log(Elog::debug, client.address(),
				": Request parsing finished; ", buf.len(), " trailing bytes.");
			if (buf.len() > 0) // deliver trailing bytes to worker
				return (_deliver(client, buf));
		}
	} catch (Client::RedirectionException& e) {
		logging::elog.log(Elog::error, client.address(),
			": Verkeerd verbonden: ", e.what());
		return (IOStatus::failure);
	} catch (Client::HTTPErrorException& e) {
		logging::elog.log(Elog::error, client.address(),
			": An error happened: ", e.what());
		return (IOStatus::failure);
	} catch (Client::BodySizeException& e) {
		logging::elog.log(Elog::error, client.address(),
			": That didn't work: ", e.what());
		// respond Payload Too Large
		return (IOStatus::failure);
	} catch (http::parse::VersionException& e) {
		logging::elog.log(Elog::error, client.address(),
			": Parse error: ", e.what());
		client.respond({http::Status::version_not_supported, virtual_server(client)});
		return (IOStatus::failure);
	} catch (http::parse::Exception& e) {
		logging::elog.log(Elog::error, client.address(),
			": Parse error: ", e.what());
		client.respond({http::Status::bad_request, virtual_server(client)});
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}

Server::IOStatus
Server::_parse_response(Client& client) {
	webserv::Buffer	buf;

	if (_fetch(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	if (buf.len() == 0)
		return (IOStatus::success);
	logging::elog.log(Elog::debug, client.address(),
		": Directing ", buf.len(), " bytes to response parser.");
	try {
		if (client.parse_response(buf)) {
			logging::elog.log(Elog::debug, client.address(),
				": Response parsing from CGI finished.");
		}
	} catch (http::parse::Exception& e) {
		logging::elog.log(Elog::error, client.address(),
			": CGI parsing error: ", e.what());
		client.respond({http::Status::internal_error, virtual_server(client)});
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}

Server::IOStatus
Server::_dechunk(Client&) {
	return (IOStatus::failure); // placeholder
}

Server::IOStatus
Server::_deliver(Client& client) {
	webserv::Buffer	buf;

	if (_recv(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	try {
		switch (client.dechunk_and_deliver(buf)) {
		case Client::OperationStatus::success:
		case Client::OperationStatus::pending:
			logging::elog.log(Elog::debug, client.address(),
				": Dechunked and delivered ", buf.len(), " bytes.");
			return (IOStatus::success);
		case Client::OperationStatus::failure:
			logging::elog.log(Elog::error, client.address(),
				": Error delivering resource.");
			// todo: inject error message into body
			return (IOStatus::failure);
		default: // timeout
			__builtin_unreachable();
		}
	} catch (http::Dechunker::Exception& e) {
		logging::elog.log(Elog::error, client.address(),
			": invalid HTTP 1.1 chunk: ", e.what(), ".");
		return (IOStatus::failure);
	}
}

Server::IOStatus
Server::_recv_and_deliver(Client& client) {
	webserv::Buffer	buf;

	if (_recv(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	return (_deliver(client, buf));
}

Server::IOStatus
Server::_deliver(Client& client, webserv::Buffer const& buf) {
	switch (client.deliver(buf)) {
	case Client::OperationStatus::success:
	case Client::OperationStatus::pending:
		logging::elog.log(Elog::debug, client.address(),
			": Delivered ", buf.len(), " bytes.");
		return (IOStatus::success);
	case Client::OperationStatus::failure:
		logging::elog.log(Elog::error, client.address(),
			": Error delivering resource.");
		// todo: inject error message into body
		return (IOStatus::failure);
	default: // timeout
		__builtin_unreachable();
	}
}

Server::IOStatus
Server::_fetch_and_send(Client& client) {
	webserv::Buffer	buf;

	if (_fetch(client, buf) == IOStatus::failure)
		return (IOStatus::failure);
	return (_send(client, buf));
}

Server::IOStatus
Server::_enchunk_and_send(Client& client) {
	return (_fetch_and_send(client)); // placeholder
}

Server::IOStatus
Server::_fetch(Client& client, webserv::Buffer& buf) {
	switch (client.fetch(buf)) {
	case Client::OperationStatus::success:
		logging::elog.log(Elog::debug, client.address(),
			": Resource fetched successfully.");
		return (Server::IOStatus::success);
	case Client::OperationStatus::pending:
		if (buf.len() > 0)
			logging::elog.log(Elog::debug, client.address(),
				": Fetched ", buf.len(), " bytes.");
		return (Server::IOStatus::success);
	case Client::OperationStatus::timeout:
		logging::elog.log(Elog::error, client.address(),
			": Timeout occurred whilst fetching resource.");
		return (IOStatus::failure);
	case Client::OperationStatus::failure:
		logging::elog.log(Elog::error, client.address(),
			": Error fetching resource.");
		// inject error message into body?
		return (IOStatus::failure);
	default:
		__builtin_unreachable();
	}
}

Server::IOStatus
Server::_recv(Client& client, webserv::Buffer& buf) {
	try {
		size_t const	bytes = client.socket().read(buf);

		if (bytes == 0)
			return (IOStatus::failure);
		logging::elog.log(Elog::debug, client.address(),
			": Received ", bytes, " bytes.");
	} catch (network::Exception& e) {
		logging::elog.log(Elog::error, client.address(),
			": Networking failure: ", e.what());
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}

Server::IOStatus
Server::_send(Client& client, webserv::Buffer const& buf) {
	if (buf.len() == 0)
		return (IOStatus::success);
	try {
		size_t const	bytes = client.socket().write(buf);

		if (bytes != buf.len())
			return (IOStatus::failure);
		logging::elog.log(Elog::debug, client.address(),
			": Sent ", bytes, " bytes.");
	} catch (network::Exception& e) {
		logging::elog.log(Elog::error, client.address(),
			": Networking failure: ", e.what());
		return (IOStatus::failure);
	}
	return (IOStatus::success);
}

static void
validate_body_size(Client const& client, size_t max) {
	std::string	value;
	if (max > 0) {
		try {
			value = client.request().headers().at("Content-Length").csvalue();
		} catch (std::out_of_range&) {
			return;
		}
		try {
			if (std::stoul(value) > max)
				throw (Client::BodySizeException());
		} catch (std::exception&) { // faulty header
			throw (Client::BodySizeException());
		}
	}
}