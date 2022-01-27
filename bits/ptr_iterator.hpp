#pragma once

#include "../iterator.hpp"

#include <algorithm>

// YEP. NESTED NAMESPACE LIKE ft::__clsaad_impl DOES NOT EXIST BEFORE C++11

namespace ft {
	namespace __clsaad_impl {
		template<typename _Tp, typename _Container>
		class ptr_iterator {
		public:
			typedef _Tp value_type;
			typedef std::ptrdiff_t difference_type;
			typedef _Tp *pointer;
			typedef _Tp &reference;
			typedef std::random_access_iterator_tag iterator_category;

		private:
			pointer ptr;

		public:

			ptr_iterator(): ptr(NULL) {}
			ptr_iterator(pointer ptr): ptr(ptr) {}
			ptr_iterator(const ptr_iterator &cpy): ptr(cpy.ptr) {}
			~ptr_iterator() {}

			ptr_iterator &operator=(const ptr_iterator &rhs) {
				ptr = rhs.ptr;
				return (*this);
			}

			bool operator==(const ptr_iterator &rhs) const {
				return ptr == rhs.ptr;
			}

			bool operator!=(const ptr_iterator &rhs) const {
				return !(*this == rhs);
			}

			reference operator*() const {
				return *ptr;
			}

			pointer operator->() const {
				return ptr;
			}

			ptr_iterator &operator++() {
				++ptr;
				return (*this);
			}

			ptr_iterator operator++(int) {
				ptr_iterator<_Tp, _Container> cpy(*this);

				++(*this);

				return cpy;
			}

			ptr_iterator &operator--() {
				--ptr;

				return (*this);
			}

			ptr_iterator operator--(int) {
				ptr_iterator<_Tp, _Container> cpy(*this);

				--(*this);

				return cpy;
			}

			ptr_iterator &operator+=(difference_type n) {
				ptr += n;

				return (*this);
			}

			ptr_iterator operator+(difference_type n) const {
				ptr_iterator<_Tp, _Container> cpy(*this);

				cpy += n;

				return cpy;
			}

			ptr_iterator &operator-=(difference_type n) {
				ptr -= n;

				return (*this);
			}

			ptr_iterator operator-(difference_type n) const {
				ptr_iterator<_Tp, _Container> cpy(*this);

				cpy -= n;

				return cpy;
			}

			difference_type operator-(ptr_iterator<_Tp, _Container> rhs) const {
				return ptr - rhs.ptr;
					// static_cast<difference_type>((reinterpret_cast<std::size_t>(ptr)
					// 	- reinterpret_cast<std::size_t>(rhs.ptr))
					// / sizeof(*ptr));
			}

			value_type &operator[](difference_type n) const {
				return ptr[n];
			}

			bool operator<(ptr_iterator<_Tp, _Container> rhs) {
				return ptr < rhs.ptr;
			}

			bool operator<=(ptr_iterator<_Tp, _Container> rhs) {
				return !(*this > rhs);
			}

			bool operator>(ptr_iterator<_Tp, _Container> rhs) {
				return rhs < (*this);
			}

			bool operator>=(ptr_iterator<_Tp, _Container> rhs) {
				return !(*this < rhs);
			}

		};

		template<typename _Tp, typename _Container>
		ptr_iterator<_Tp, _Container> operator+(typename _Container::difference_type lhs, const ptr_iterator<_Tp, _Container> &rhs) {
			return rhs + lhs;
		}
	} // namespace __clsaad_impl
} // namespace ft
