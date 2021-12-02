#pragma once

#ifndef __FT_BITS_INC
# error "You included a bits header"
#endif

namespace ft {
	template<typename _Tp>
	struct is_integral: integral_constant<bool, false> {};

	template<>
	struct is_integral<bool>: integral_constant<bool, true> {};

	template<>
	struct is_integral<char>: integral_constant<bool, true> {};

	template<>
	struct is_integral<unsigned char>: integral_constant<bool, true> {};

	template<>
	struct is_integral<signed char>: integral_constant<bool, true> {};

	template<>
	struct is_integral<short int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<unsigned short int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<unsigned int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<long int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<unsigned long int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<long long int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<unsigned long long int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const bool>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const char>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const unsigned char>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const signed char>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const short int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const unsigned short int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const unsigned int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const long int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const unsigned long int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const long long int>: integral_constant<bool, true> {};

	template<>
	struct is_integral<const unsigned long long int>: integral_constant<bool, true> {};
} // namespace ft
