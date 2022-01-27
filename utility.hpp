#pragma once

namespace ft {
	template<typename _T0, typename _T1>
	struct pair {
		typedef _T0 first_type;
		typedef _T1 second_type;

		_T0 first;
		_T1 second;

		pair() {}

		pair(const _T0 &a, const _T1 &b): first(a), second(b) {}

		template<typename _U0, typename _U1>
		pair(const pair<_U0, _U1> &p): first(p.first), second(p.second) {}

		// Is implicitly defined until C++11 as stated in https://en.cppreference.com/w/cpp/utility/pair/pair and in https://www.cplusplus.com/reference/utility/pair/pair/
		// pair(const pair &other): first(other.first), second(other.second) {}

		pair &operator=(const pair &rhs) {
			first = rhs.first;
			second = rhs.second;

			return (*this);
		}
	};

	template<typename _T0, typename _T1>
	bool operator==(const pair<_T0, _T1> &lhs, const pair<_T0, _T1> &rhs) {
		return
			lhs.first == rhs.first &&
			lhs.second == rhs.second;
	}

	template<typename _T0, typename _T1>
	bool operator!=(const pair<_T0, _T1> &lhs, const pair<_T0, _T1> &rhs) {
		return !(lhs == rhs);
	}

	template<typename _T0, typename _T1>
	bool operator<(const pair<_T0, _T1> &lhs, const pair<_T0, _T1> &rhs) {
		if (lhs.first != rhs.first) {
			return lhs.first < rhs.first;
		}
		return lhs.second < rhs.second;
	}

	template<typename _T0, typename _T1>
	bool operator<=(const pair<_T0, _T1> &lhs, const pair<_T0, _T1> &rhs) {
		return (lhs < rhs) || (lhs == rhs);
	}

	template<typename _T0, typename _T1>
	bool operator>(const pair<_T0, _T1> &lhs, const pair<_T0, _T1> &rhs) {
		return !(lhs <= rhs);
	}

	template<typename _T0, typename _T1>
	bool operator>=(const pair<_T0, _T1> &lhs, const pair<_T0, _T1> &rhs) {
		return !(lhs < rhs);
	}

	template<typename _T0, typename _T1>
	pair<_T0, _T1> make_pair(_T0 first, _T1 second) {
		return pair<_T0, _T1>(first, second);
	}
} // namespace ft
