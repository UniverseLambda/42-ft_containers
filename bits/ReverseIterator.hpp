#pragma once

#ifndef __FT_BITS_INC
# error "You included a bits header"
#endif

#include "../iterator.hpp"

namespace ft {
	template<typename _Iter>
	struct reverse_iterator {
		typedef _Iter iterator_type;
		typedef typename iterators_traits<_Iter>::iterator_category iterator_category;
		typedef typename iterators_traits<_Iter>::value_type value_type;
		typedef typename iterators_traits<_Iter>::difference_type difference_type;
		typedef typename iterators_traits<_Iter>::pointer pointer;
		typedef typename iterators_traits<_Iter>::reference reference;
	protected:
		iterator_type current;

	public:
		reverse_iterator() {}

		explicit reverse_iterator(iterator_type it): current(it) {}

		template<typename _CpyIter>
		reverse_iterator(const reverse_iterator<_CpyIter> &cpy): current(cpy) {}

		template<typename _RhsIter>
		reverse_iterator &operator=(const reverse_iterator<_RhsIter> &rhs) {
			current = rhs.current;

			return (*this);
		}

		iterator_type base() {
			return current;
		}

		reference operator*() const {
			return &*(current - 1);
		}

		pointer operator->() const {
			return &(operator*());
		}

		reference operator[](difference_type n) const {
			return current[n - 1];
		}

		reverse_iterator &operator++() {
			--current;
			return (*this);
		}

		reverse_iterator &operator--() {
			++current;
			return (*this);
		}

		reverse_iterator operator++(int) {
			reverse_iterator res(*this);

			--current;
			return (*this);
		}

		reverse_iterator operator--(int) {
			reverse_iterator res(*this);

			++current;
			return (*this);
		}

		reverse_iterator operator+(difference_type n) const {
			reverse_iterator res(current - n);

			return res;
		}

		reverse_iterator operator-(difference_type n) const {
			reverse_iterator res(current + n);

			return res;
		}

		reverse_iterator operator+=(difference_type n) const {
			current -= n;

			return (*this);
		}

		reverse_iterator operator-=(difference_type n) const {
			current += n;
			return (*this);
		}
	};

	template<typename _Iterator0, typename _Iterator1>
	bool operator==(const reverse_iterator<_Iterator0> &lhs, const reverse_iterator<_Iterator1> &rhs) {
		return lhs.base() == rhs.base();
	}

	template<typename _Iterator0, typename _Iterator1>
	bool operator!=(const reverse_iterator<_Iterator0> &lhs, const reverse_iterator<_Iterator1> &rhs) {
		return lhs.base() != rhs.base();
	}

	template<typename _Iterator0, typename _Iterator1>
	bool operator<=(const reverse_iterator<_Iterator0> &lhs, const reverse_iterator<_Iterator1> &rhs) {
		return lhs.base() <= rhs.base();
	}

	template<typename _Iterator0, typename _Iterator1>
	bool operator>=(const reverse_iterator<_Iterator0> &lhs, const reverse_iterator<_Iterator1> &rhs) {
		return lhs.base() >= rhs.base();
	}

	template<typename _Iterator0, typename _Iterator1>
	bool operator<(const reverse_iterator<_Iterator0> &lhs, const reverse_iterator<_Iterator1> &rhs) {
		return lhs.base() < rhs.base();
	}

	template<typename _Iterator0, typename _Iterator1>
	bool operator>(const reverse_iterator<_Iterator0> &lhs, const reverse_iterator<_Iterator1> &rhs) {
		return lhs.base() > rhs.base();
	}

	template<typename _Iterator>
	reverse_iterator<_Iterator> operator+(typename reverse_iterator<_Iterator>::difference_type lhs, const reverse_iterator<_Iterator> &rhs)  {
		return rhs + lhs;
	}

	template<typename _Iterator>
	typename reverse_iterator<_Iterator>::difference_type operator-(const reverse_iterator<_Iterator> &lhs, const reverse_iterator<_Iterator> &rhs)  {
		return rhs.base() - lhs.base();
	}
} // namespace ft
