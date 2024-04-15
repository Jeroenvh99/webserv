#ifndef NETWORK_HANDLE_HPP
# define NETWORK_HANDLE_HPP

# include "network.hpp"

# include <memory>
# include <stdexcept>

namespace network {
	class Handle {
	public:
		class Exception;
		using Raw = int;

		static constexpr Raw	_invalid_handle = -1;

		Handle();
		virtual ~Handle();
		Handle(Raw);
		Handle(Handle const&);
		Handle(Handle&&) noexcept;
		Handle&	operator=(Handle const&) ;
		Handle&	operator=(Handle&&) noexcept;

		bool	good() const noexcept;

		void	close();

	protected:
		bool	operator==(Raw const&) const noexcept;
		bool	operator!=(Raw const&) const noexcept;

		Raw	raw() const noexcept;

	private:
		friend class Poller;
		Raw	release() noexcept;

		Raw	_raw;
	}; // class Handle

	using SharedHandle = std::shared_ptr<Handle>;

	class Handle::Exception: public network::Exception {
	public:
		Exception(char const*);
	}; // class Handle::Exception
}; // namespace Network

#endif // NETWORK_FILE_HANDLE_HPP
