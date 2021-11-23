#pragma once

#include <memory>

#include "bits/PtrIterator.hpp"

namespace ft {
	template<typename _Tp, typename _Allocator = std::allocator<_Tp> >
	class vector {
	public:
		typedef _Tp value_type;
		typedef _Allocator allocator_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef typename _Allocator::pointer pointer;
		typedef typename _Allocator::const_pointer const_pointer;
		typedef __clsaad_impl::PtrIterator<value_type, vector<value_type, allocator_type> > iterator;
		typedef __clsaad_impl::PtrIterator<const value_type, vector<value_type, allocator_type> > const_iterator;

		// FIXME: Merge reverse_iterator changes
		// typedef _Allocator reverse_iterator;
		// typedef _Allocator const_reverse_iterator;

	private:
		allocator_type mAlloc;
		pointer mData;
		size_type mSize;
		size_type mCapacity;

	public:
		vector():
			mAlloc(),
			mData(mAlloc.allocate(0)),
			mSize(0),
			mCapacity(0) {}

		explicit vector(const allocator_type &alloc):
			mAlloc(alloc),
			mData(mAlloc.allocate(0)),
			mSize(0),
			mCapacity(0) {}

		explicit vector(size_type count, const value_type &value = value_type(), const allocator_type &alloc = allocator_type()):
			mAlloc(alloc),
			mData(mAlloc.allocate(count)),
			mSize(count),
			mCapacity(count) {
			for (size_type i = 0; i < count; ++i) {
				mAlloc.construct(&(mData[i]), value);
			}
		}

		explicit vector(size_type count):
			mAlloc(),
			mData(mAlloc.allocate(count)),
			mSize(count),
			mCapacity(count) {
			for (size_type i = 0; i < count; ++i) {
				mAlloc.construct(&(mData[i]));
			}
		}

		// template<typename _InputIt>
		// vector(_InputIt first, _InputIt last, const allocator_type &alloc = allocator_type()):
		// 	mAlloc(alloc), mData(mAlloc.allocate(0)), mSize(0), mCapacity(0) {
		// 	for (; first != last; ++first) {
		// 		push_back(*first);
		// 	}
		// }

		vector(const vector &other): mAlloc(other.mAlloc) {
			mData = mAlloc(other.mCapacity);
			mSize = other.mSize;
			mCapacity = other.mCapacity;

			for (size_type i = 0; i < mSize; ++i) {
				mAlloc.construct(&(mData[i]), other.mData[i]);
			}
		}
	};
} // namespace ft
