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
} // namespace ft

#define __FT_BITS_INC
#include "bits/is_integral.hpp"
#undef __FT_BITS_INC
