#pragma once

#include "../iterator.hpp"

#include <algorithm>

// YEP. NESTED NAMESPACE LIKE ft::__clsaad_impl DOES NOT EXIST BEFORE C++11

namespace ft {
	namespace __clsaad_impl {
		template<typename _Tp, typename _Container>
		class PtrIterator: public ft::iterator<typename ft::random_access_iterator_tag, _Tp> {
		private:
			typedef ft::iterator_traits<PtrIterator> _ItTrait;

			typename _ItTrait::pointer mPtr;

		public:

			PtrIterator(): mPtr(NULL) {}
			PtrIterator(typename _ItTrait::pointer ptr): mPtr(ptr) {}
			PtrIterator(const PtrIterator &cpy): mPtr(cpy.mPtr) {}
			~PtrIterator() {}

			PtrIterator &operator=(const PtrIterator &rhs) {
				mPtr = rhs.mPtr;
			}

			bool operator==(const PtrIterator &rhs) const {
				return mPtr == rhs.mPtr;
			}

			bool operator!=(const PtrIterator &rhs) const {
				return !(*this == rhs);
			}

			typename _ItTrait::reference operator*() const {
				return *mPtr;
			}

			typename _ItTrait::pointer operator->() const {
				return mPtr;
			}

			PtrIterator &operator++() {
				++mPtr;
				return (*this);
			}

			PtrIterator operator++(int) {
				PtrIterator<_Tp, _Container> cpy(*this);

				++(*this);

				return cpy;
			}

			PtrIterator &operator--() {
				--mPtr;

				return (*this);
			}

			PtrIterator operator--(int) {
				PtrIterator<_Tp, _Container> cpy(*this);

				--(*this);

				return cpy;
			}

			PtrIterator &operator+=(typename _ItTrait::difference_type n) {
				mPtr += n;

				return (*this);
			}

			PtrIterator operator+(typename _ItTrait::difference_type n) {
				PtrIterator<_Tp, _Container> cpy(*this);

				cpy += n;

				return cpy;
			}

			PtrIterator &operator-=(typename _ItTrait::difference_type n) {
				mPtr -= n;

				return (*this);
			}

			PtrIterator operator-(typename _ItTrait::difference_type n) {
				PtrIterator<_Tp, _Container> cpy(*this);

				cpy -= n;

				return cpy;
			}

			typename _ItTrait::difference_type operator-(PtrIterator<_Tp, _Container> rhs) {
				return
					(reinterpret_cast<typename _ItTrait::difference_type>(mPtr)
						- reinterpret_cast<typename _ItTrait::difference_type>(rhs.mPtr))
					/ sizeof(*mPtr);
			}

			typename _ItTrait::value_type &operator[](typename _ItTrait::difference_type n) {
				return mPtr[n];
			}

			bool operator<(PtrIterator<_Tp, _Container> rhs) {
				return mPtr < rhs.mPtr;
			}

			bool operator<=(PtrIterator<_Tp, _Container> rhs) {
				return !(*this > rhs);
			}

			bool operator>(PtrIterator<_Tp, _Container> rhs) {
				return rhs < (*this);
			}

			bool operator>=(PtrIterator<_Tp, _Container> rhs) {
				return !(*this < rhs);
			}
		};
	} // namespace __clsaad_impl
} // namespace ft
