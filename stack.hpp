#pragma once

#include "vector.hpp"

namespace ft {
	template< typename _Tp, typename _Container = ft::vector<_Tp> >
	class stack {
	public:
		typedef _Container container_type;
		typedef typename _Container::value_type value_type;
		typedef typename _Container::size_type size_type;
		typedef typename _Container::reference reference;
		typedef typename _Container::const_reference const_reference;

	protected:
		_Container c;

	public:
		explicit stack(const _Container &cont = _Container()): c(cont) {}

		stack(const stack &other): c(other.c) {}

		// Well, nothing to do.
		~stack() {}

		stack &operator=(const stack &rhs)  {
			c = rhs.c;
			return (*this);
		}

		reference top() {
			return c.back();
		}

		const_reference top() const {
			return c.back();
		}

		bool empty() const {
			return c.empty();
		}

		size_type size() const {
			return c.size();
		}

		void push(const value_type &value) {
			c.push_back(value);
		}

		void pop() {
			c.pop_back();
		}
	};

	template<typename _Tp, typename _Container>
	bool operator==(const ft::stack<_Tp, _Container> &lhs, const ft::stack<_Tp, _Container> &rhs) {
		return lhs == rhs;
	}

	template<typename _Tp, typename _Container>
	bool operator!=(const ft::stack<_Tp, _Container> &lhs, const ft::stack<_Tp, _Container> &rhs) {
		return lhs != rhs;
	}

	template<typename _Tp, typename _Container>
	bool operator<(const ft::stack<_Tp, _Container> &lhs, const ft::stack<_Tp, _Container> &rhs) {
		return lhs < rhs;
	}

	template<typename _Tp, typename _Container>
	bool operator<=(const ft::stack<_Tp, _Container> &lhs, const ft::stack<_Tp, _Container> &rhs) {
		return lhs <= rhs;
	}

	template<typename _Tp, typename _Container>
	bool operator>(const ft::stack<_Tp, _Container> &lhs, const ft::stack<_Tp, _Container> &rhs) {
		return lhs > rhs;
	}

	template<typename _Tp, typename _Container>
	bool operator>=(const ft::stack<_Tp, _Container> &lhs, const ft::stack<_Tp, _Container> &rhs) {
		return lhs >= rhs;
	}
} // namespace ft
