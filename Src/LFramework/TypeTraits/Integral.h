#pragma once

#include <type_traits>
#include <limits>
#include <cstddef>
#include <algorithm>

namespace LFramework::TypeTraits::Integral {

	template<typename T, typename = ::std::enable_if_t<::std::is_integral_v<T>>>
	static constexpr std::size_t digitsCount(T value) {
		return value == 0 ? 0 : 1 + digitsCount(value / 10);
	}

	template<typename T>
	static constexpr std::size_t digitsCountBase10(T value) {
		std::size_t result = 0;
		while (value != 0) {
			++result;
			value /= 10;
		}
		return result;
	}

	template<typename T, typename = ::std::enable_if_t<::std::is_integral_v<T>>>
	static constexpr std::size_t maxDigitsCount() {
	    return (::std::max)(digitsCount((::std::numeric_limits<T>::min)()), digitsCount((::std::numeric_limits<T>::max)()));
	}


}
