#pragma once

#include <memory>

#include "bits/PtrIterator.hpp"

#include "iterator.hpp"

#include "algorithm.hpp"

#include <stdexcept>
#include <limits>

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
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

	private:
		allocator_type mAlloc;
		size_type mCapacity;
		pointer mData;
		size_type mSize;

	public:
		vector():
			mAlloc(),
			mCapacity(16),
			mData(mAlloc.allocate(mCapacity)),
			mSize(0) {}

		explicit vector(const allocator_type &alloc):
			mAlloc(alloc),
			mCapacity(16),
			mData(mAlloc.allocate(mCapacity)),
			mSize(0) {}

		explicit vector(size_type count, const value_type &value = value_type(), const allocator_type &alloc = allocator_type()):
			mAlloc(alloc),
			mCapacity(calc_capacity(count)),
			mData(mAlloc.allocate(mCapacity)),
			mSize(count) {

			for (size_type i = 0; i < count; ++i) {
				mAlloc.construct(&(mData[i]), value);
			}
		}

		explicit vector(size_type count):
			mAlloc(),
			mCapacity(calc_capacity(count)),
			mData(mAlloc.allocate(mCapacity)),
			mSize(count) {

			for (size_type i = 0; i < count; ++i) {
				mAlloc.construct(&(mData[i]));
			}
		}

		template<typename _InputIt>
		vector(_InputIt first, _InputIt last, const allocator_type &alloc = allocator_type()):
			mAlloc(alloc),
			mCapacity(8),
			mData(mAlloc.allocate(mCapacity)),
			mSize(0) {
			for (; first != last; ++first) {
				push_back(*first);
			}
		}

		vector(const vector &other): mAlloc(other.mAlloc) {
			mData = mAlloc.allocate(other.mCapacity);
			mSize = other.mSize;
			mCapacity = other.mCapacity;

			for (size_type i = 0; i < mSize; ++i) {
				mAlloc.construct(&(mData[i]), other.mData[i]);
			}
		}

		~vector() {
			free_data();
		}

		vector &operator=(const vector &rhs) {
			free_data();

			mAlloc = rhs.mAlloc;
			mData = mAlloc.allocate(rhs.mCapacity);
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;


			for (size_type i = 0; i < mSize; ++i) {
				mAlloc.construct(&mData[i], rhs.mData[i]);
			}

			return (*this);
		}

		void assign(size_type count, const _Tp &value) {
			size_type validCopies = ensure_replacing_allocation(count);
			for (size_type i = 0; i < mSize; ++i) {
				if (i < validCopies) {
					mData[i] = value;
				} else {
					mAlloc.allocate(&(mData[i]), value);
				}
			}
		}

		template<typename _InputIt>
		void assign(_InputIt first, _InputIt last) {
			size_type previousSize = mSize;

			mSize = 0;
			for (size_type i = 0; i < previousSize && first != last; ++first, ++i) {
				mData[i] = *first;
			}

			for (; first != last; ++first) {
				push_data(*first);
			}

			while (previousSize-- > mSize) {
				mAlloc.destroy(&(mData[previousSize]));
			}
		}

		allocator_type get_allocator() const {
			return mAlloc;
		}

		reference at(size_type idx) {
			if (idx >= mSize) {
				throw std::out_of_range("Vector::at: idx out-of-range");
			}

			return mData[idx];
		}

		const_reference at(size_type idx) const {
			if (idx >= mSize) {
				throw std::out_of_range("Vector::at: idx out-of-range");
			}

			return mData[idx];
		}

		reference operator[](size_type idx) {
			return mData[idx];
		}

		const_reference operator[](size_type idx) const {
			return mData[idx];
		}

		reference front() {
			return mData[0];
		}

		const_reference front() const {
			return mData[0];
		}

		reference back() {
			return mData[mSize - 1];
		}

		const_reference back() const {
			return mData[mSize - 1];
		}

		_Tp *data() {
			return mData;
		}

		const _Tp *data() const {
			return mData;
		}

		iterator begin() {
			return iterator(mData);
		}

		const_iterator begin() const {
			return const_iterator(mData);
		}

		iterator end() {
			return iterator(mData + mSize);
		}

		const_iterator end() const {
			return const_iterator(mData + mSize);
		}

		iterator rbegin() {
			return reverse_iterator(end());
		}

		const_iterator rbegin() const {
			return const_reverse_iterator(end());
		}

		iterator rend() {
			return reverse_iterator(begin());
		}

		const_iterator rend() const {
			return reverse_iterator(begin());
		}

		bool empty() const {
			return (mSize == 0);
		}

		size_type size() const {
			return mSize;
		}

		size_type max_size() const {
			return std::numeric_limits<difference_type>::max();
		}

		void reserve(size_type newCapacity) {
			if (newCapacity <= mCapacity) {
				return;
			}

			if (newCapacity >= max_size()) {
				throw std::length_error("Vector::reserve: requested capacity greater than max_size()");
			}

			newCapacity = calc_capacity(newCapacity);
			_Tp *newData = mAlloc.allocate(newCapacity);

			move_data(newData);

			mAlloc.deallocate(mData, mCapacity);

			mData = newData;
			mCapacity = newCapacity;
		}

		size_type capacity() const {
			return mCapacity;
		}

		void clear() {
			for (size_type i = 0; i < mSize; ++i) {
				mAlloc.destroy(&mData[i]);
			}

			mSize = 0;
		}

		iterator insert(iterator pos, const _Tp &value) {
			return insert(pos, 1, value);
		}

		void insert(iterator pos, size_type count, const _Tp &value) {
			size_type idx = pos - begin();

			make_hole(idx, count);

			mAlloc.construct(&mData[idx], value);
		}

		template<typename _InputIt>
		void insert(iterator pos, _InputIt first, _InputIt last) {
			for (size_type idx = pos - begin(); first != last; ++idx, ++first) {
				make_hole(idx, 1);

				mAlloc.construct(&mData[idx], *first);
			}
		}

		iterator erase(iterator pos) {
			for (size_type i = pos - begin(); i < (mSize - 1); ++i) {
				mData[i] = mData[i + 1];
			}

			--mSize;

			mAlloc.destroy(&mData[mSize]);

			// Yeah, nice trick lmao
			return pos;
		}

		iterator erase(iterator first, iterator last) {
			size_type erased = last - first;
			size_type idx = first - begin();

			for (size_type i = idx + erased; i < mSize; ++i) {
				mData[i - erased] = mData[i];
			}

			for (size_type i = 0; i < erased; ++i) {
				mAlloc.destroy(&mData[(mSize - erased) + i]);
			}

			mSize -= erased;

			return iterator(mData + idx);
		}

		void push_back(const _Tp &value) {
			reserve(size() + 1);

			mAlloc.construct(&mData[mSize], value);
			++mSize;
		}

		void pop_back() {
			--mSize;
			mAlloc.destroy(&mData[mSize]);
		}

		void resize(size_type count, _Tp value = _Tp()) {
			reserve(count);
			if (count < mSize) {
				for (size_type i = count; i < mSize; ++i) {
					pop_back();
				}
			} else {
				for (size_type i = mSize; i < count; ++i) {
					push_back(value);
				}
			}
		}

		void swap(vector &other) {
			allocator_type allocator;
			size_type capacity;
			pointer data;
			size_type size;

			allocator = mAlloc;
			capacity = mCapacity;
			data = mData;
			size = mSize;

			mAlloc = other.mAlloc;
			mCapacity = other.mCapacity;
			mData = other.mData;
			mSize = other.mSize;

			other.mAlloc = allocator;
			other.mCapacity = capacity;
			other.mData = data;
			other.mSize = size;
		}

	private:
		size_type calc_capacity(size_type capacity) {
			size_type adjustedCapacity;
#if defined(__GNUC__) || defined(__clang__)
			adjustedCapacity = (1 << (64 - __builtin_clzl(capacity - 1)));
#else
			adjustedCapacity = mCapacity;

			while (adjustedCapacity < capacity) {
				adjustedCapacity << 2;
			}
#endif
			return (adjustedCapacity > max_size()) ? capacity : adjustedCapacity;
		}

		void free_data() {
			for (size_type i = 0; i < mSize; ++i) {
				mAlloc.destroy(&(mData[i]));
			}
			mAlloc.deallocate(mData, mCapacity);
		}

		void make_hole(size_type idx, size_type holeSize = 1) {
			if (mSize + holeSize > mCapacity) {
				size_type newCapacity = calc_capacity(mSize + holeSize);

				_Tp *newData = mAlloc.allocate(newCapacity);

				for (size_type i = 0; i < mSize; ++i) {
					mAlloc.construct(&newData[i + (holeSize * (i >= idx))], mData[i]);
					mAlloc.destroy(&mData[i]);
				}

				mAlloc.deallocate(mData, mCapacity);

				mCapacity = newCapacity;
				mData = newData;
			} else {
				for (size_type i = mSize; i > idx; --i) {
					mAlloc.construct(&mData[(i - 1) + holeSize], mData[(i - 1)]);
				}

				for (size_type i = 0; i < holeSize; ++i) {
					mAlloc.destroy(&mData[idx + i]);
				}
			}
			mSize += holeSize;
		}

		void move_data(_Tp *newData) {
			for (size_type i = 0; i < mSize; ++i) {
				mAlloc.construct(&newData[i], mData[i]);
				mAlloc.destroy(&mData[i]);
			}
		}

		/**
		 * @return true if mData was reallocated (so it contains only stall memory);
		 * 	false mData was reused (so it still contains valid copies)
		 */
		size_type ensure_replacing_allocation(size_type count) {
			if (mCapacity < count) {
				free_data();
				mCapacity = calc_capacity(count);
				mAlloc.allocate(mCapacity);
				mSize = 0;
				return 0;
			}

			for (size_type i = count; i < mSize; ++i) {
				mAlloc.destroy(&(mData[i]));
			}

			size_type retValue = (mSize < count) ? mSize : count;
			mSize = count;
			return retValue;
		}
	};

	template<typename _Tp, typename _Alloc>
	bool operator==(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		if (v0.size() != v1.size()) {
			return false;
		}

		return equal(v0.begin(), v0.end(), v1.begin());
	}

	template<typename _Tp, typename _Alloc>
	bool operator!=(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		return !(v0 == v1);
	}

	template<typename _Tp, typename _Alloc>
	bool operator<(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		typedef typename vector<_Tp, _Alloc>::const_iterator iter;

		iter it0 = v0.begin();
		iter it1 = v1.begin();

		for (; it0 != v0.end() && it1 != v1.end(); ++it0, ++it1) {
			if (!(*it0 < *it1)) {
				return false;
			}
		}

		return it0 == v0.end() && it1 != v1.end();
	}

	template<typename _Tp, typename _Alloc>
	bool operator<=(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		typedef typename vector<_Tp, _Alloc>::const_iterator iter;

		iter it0 = v0.begin();
		iter it1 = v1.begin();

		for (; it0 != v0.end() && it1 != v1.end(); ++it0, ++it1) {
			if (*it1 < *it0) {
				return false;
			}
		}

		return it0 == v0.end();
	}

	template<typename _Tp, typename _Alloc>
	bool operator>(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		return v1 < v0;
	}

	template<typename _Tp, typename _Alloc>
	bool operator>=(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		return v1 <= v0;
	}

	template<typename _Tp, typename _Alloc>
	void swap(vector<_Tp, _Alloc> &v0, vector<_Tp, _Alloc> &v1) {
		v0.swap(v1);
	}
} // namespace ft
