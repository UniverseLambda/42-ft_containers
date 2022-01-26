#pragma once

namespace ft {
	template<bool B, class _Tp = void>
	struct enable_if;

	template<class _Tp>
	struct enable_if<true, _Tp> {
		typedef _Tp type;
	};

	template<class _Tp, _Tp _Tvalue>
	struct integral_constant {
		typedef _Tp value_type;
		typedef integral_constant<_Tp, _Tvalue> type;

		static const _Tp value = _Tvalue;

		operator value_type() const throw() {
			return value;
		}

		value_type operator()() const throw()  {
			return value;
		}
	};

	typedef integral_constant<bool, true> true_type;
	typedef integral_constant<bool, false> false_type;

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
