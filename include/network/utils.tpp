#ifndef NETWORK_UTILS_TPP
# define NETWORK_UTILS_TPP

# include <type_traits>

namespace network {
	template<typename T>
	typename std::underlying_type<typename T::value_type>::type
	_get_bitmask(T const& ctr) {
		using Underlying = typename std::underlying_type<typename T::value_type>::type;
		
		Underlying	res = 0;

		for (auto const& flag: ctr)
			res |= static_cast<Underlying>(flag);
		return (res);
	}
}; // namespace network

#endif // NETWORK_UTILS_TPP
