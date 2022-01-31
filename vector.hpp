#pragma once

#include <memory>

#include "bits/ptr_iterator.hpp"

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
		typedef __clsaad_impl::ptr_iterator<value_type, vector<value_type, allocator_type> > iterator;
		typedef __clsaad_impl::ptr_iterator<const value_type, vector<value_type, allocator_type> > const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

	private:
		// Well, can't name just capacity, data and size (because, you know, methods), so...
		// For this container, I'll just put m_ in front of every member variable
		allocator_type m_alloc;
		size_type m_capacity;
		pointer m_data;
		size_type m_size;

	public:
		vector():
			m_alloc(),
			m_capacity(16),
			m_data(m_alloc.allocate(m_capacity)),
			m_size(0) {}

		explicit vector(const allocator_type &alloc):
			m_alloc(alloc),
			m_capacity(16),
			m_data(m_alloc.allocate(m_capacity)),
			m_size(0) {}

		explicit vector(size_type count, const value_type &value = value_type(), const allocator_type &alloc = allocator_type()):
			m_alloc(alloc),
			m_capacity(calc_capacity(count)),
			m_data(m_alloc.allocate(m_capacity)),
			m_size(count) {

			for (size_type i = 0; i < count; ++i) {
				m_alloc.construct(&m_data[i], value);
			}
		}

		explicit vector(size_type count):
			m_alloc(),
			m_capacity(calc_capacity(count)),
			m_data(m_alloc.allocate(m_capacity)),
			m_size(count) {

			for (size_type i = 0; i < count; ++i) {
				m_alloc.construct(&(m_data[i]));
			}
		}

		template<typename _InputIt>
		vector(_InputIt first, _InputIt last, const allocator_type &alloc = allocator_type()):
			m_alloc(alloc),
			m_capacity(16),
			m_data(m_alloc.allocate(m_capacity)),
			m_size(0) {
			for (; first != last; ++first) {
				push_back(*first);
			}
		}

		vector(const vector &other): m_alloc(other.m_alloc) {
			m_data = m_alloc.allocate(other.m_capacity);
			m_size = other.m_size;
			m_capacity = other.m_capacity;

			for (size_type i = 0; i < m_size; ++i) {
				m_alloc.construct(&(m_data[i]), other.m_data[i]);
			}
		}

		~vector() {
			free_data();
		}

		vector &operator=(const vector &rhs) {
			free_data();

			m_alloc = rhs.m_alloc;
			m_data = m_alloc.allocate(rhs.m_capacity);
			m_size = rhs.m_size;
			m_capacity = rhs.m_capacity;


			for (size_type i = 0; i < m_size; ++i) {
				m_alloc.construct(&m_data[i], rhs.m_data[i]);
			}

			return (*this);
		}

		void assign(size_type count, const _Tp &value) {
			size_type valid_copies = ensure_replacing_allocation(count);
			for (size_type i = 0; i < m_size; ++i) {
				if (i < valid_copies) {
					m_data[i] = value;
				} else {
					m_alloc.construct(&m_data[i], value);
				}
			}
		}

		template<typename _InputIt>
		void assign(_InputIt first, _InputIt last) {
			size_type previous_size = m_size;

			m_size = 0;
			for (size_type i = 0; i < previous_size && first != last; ++first, ++i, ++m_size) {
				m_data[i] = *first;
			}

			for (; first != last; ++first) {
				push_back(*first);
			}

			while (previous_size-- > m_size) {
				m_alloc.destroy(&(m_data[previous_size]));
			}
		}

		allocator_type get_allocator() const {
			return m_alloc;
		}

		reference at(size_type idx) {
			if (idx >= m_size) {
				throw std::out_of_range("vector::at: idx out-of-range");
			}

			return m_data[idx];
		}

		const_reference at(size_type idx) const {
			if (idx >= m_size) {
				throw std::out_of_range("vector::at: idx out-of-range");
			}

			return m_data[idx];
		}

		reference operator[](size_type idx) {
			return m_data[idx];
		}

		const_reference operator[](size_type idx) const {
			return m_data[idx];
		}

		reference front() {
			return m_data[0];
		}

		const_reference front() const {
			return m_data[0];
		}

		reference back() {
			return m_data[m_size - 1];
		}

		const_reference back() const {
			return m_data[m_size - 1];
		}

		_Tp *data() {
			return m_data;
		}

		const _Tp *data() const {
			return m_data;
		}

		iterator begin() {
			return iterator(m_data);
		}

		const_iterator begin() const {
			return const_iterator(m_data);
		}

		iterator end() {
			return iterator(m_data + m_size);
		}

		const_iterator end() const {
			return const_iterator(m_data + m_size);
		}

		reverse_iterator rbegin() {
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(end());
		}

		reverse_iterator rend() {
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const {
			return const_reverse_iterator(begin());
		}

		bool empty() const {
			return (m_size == 0);
		}

		size_type size() const {
			return m_size;
		}

		size_type max_size() const {
			return std::numeric_limits<difference_type>::max() / sizeof(value_type);
		}

		void reserve(size_type new_capacity) {
			if (new_capacity <= m_capacity) {
				return;
			}

			if (new_capacity > max_size()) {
				throw std::length_error("vector::reserve: requested capacity greater than max_size()");
			}

			new_capacity = calc_capacity(new_capacity);
			_Tp *new_data = m_alloc.allocate(new_capacity);

			move_data(new_data);

			m_alloc.deallocate(m_data, m_capacity);

			m_data = new_data;
			m_capacity = new_capacity;
		}

		size_type capacity() const {
			return m_capacity;
		}

		void clear() {
			for (size_type i = 0; i < m_size; ++i) {
				m_alloc.destroy(&m_data[i]);
			}

			m_size = 0;
		}

		iterator insert(iterator pos, const _Tp &value) {
			size_type idx = pos - begin();

			insert(pos, 1, value);
			return iterator(m_data + idx);
		}

		void insert(iterator pos, size_type count, const _Tp &value) {
			size_type idx = pos - begin();

			make_hole(idx, count);

			for (size_type i = 0; i < count; ++i) {
				m_alloc.construct(&m_data[idx + i], value);
			}
		}

		template<typename _InputIt>
		void insert(iterator pos, _InputIt first, _InputIt last) {
			for (size_type idx = pos - begin(); first != last; ++idx, ++first) {
				make_hole(idx, 1);

				m_alloc.construct(&m_data[idx], *first);
			}
		}

		iterator erase(iterator pos) {
			for (size_type i = pos - begin(); i < (m_size - 1); ++i) {
				m_data[i] = m_data[i + 1];
			}

			--m_size;

			m_alloc.destroy(&m_data[m_size]);

			// Yeah, nice trick lmao
			return pos;
		}

		iterator erase(iterator first, iterator last) {
			size_type erased = last - first;
			size_type idx = first - begin();

			for (size_type i = idx + erased; i < m_size; ++i) {
				m_data[i - erased] = m_data[i];
			}

			for (size_type i = 0; i < erased; ++i) {
				m_alloc.destroy(&m_data[(m_size - erased) + i]);
			}

			m_size -= erased;

			return iterator(m_data + idx);
		}

		void push_back(const _Tp &value) {
			reserve(size() + 1);

			m_alloc.construct(&m_data[m_size], value);
			++m_size;
		}

		void pop_back() {
			--m_size;
			m_alloc.destroy(&m_data[m_size]);
		}

		void resize(size_type count, _Tp value = _Tp()) {
			reserve(count);
			if (count < m_size) {
				while (m_size > count) {
					pop_back();
				}
			} else {
				for (size_type i = m_size; i < count; ++i) {
					push_back(value);
				}
			}
		}

		void swap(vector &other) {
			std::swap(m_alloc, other.m_alloc);
			std::swap(m_capacity, other.m_capacity);
			std::swap(m_data, other.m_data);
			std::swap(m_size, other.m_size);
		}

	private:
		size_type calc_capacity(size_type capacity) {
			size_type adjustedCapacity;
#if defined(__GNUC__) || defined(__clang__)
			adjustedCapacity = (1 << (64 - __builtin_clzl(capacity - 1)));
#else
			adjustedCapacity = 8;

			while (adjustedCapacity < capacity && adjustedCapacity != 0) {
				adjustedCapacity << 2;
			}
#endif
			return (adjustedCapacity > max_size()) ? capacity : adjustedCapacity;
		}

		void free_data() {
			for (size_type i = 0; i < m_size; ++i) {
				m_alloc.destroy(&(m_data[i]));
			}
			m_alloc.deallocate(m_data, m_capacity);
		}

		void make_hole(size_type idx, size_type holeSize = 1) {
			if (holeSize == 0) {
				return;
			}

			if (m_size + holeSize > m_capacity) {
				size_type new_capacity = calc_capacity(m_size + holeSize);

				_Tp *new_data = m_alloc.allocate(new_capacity);

				for (size_type i = 0; i < m_size; ++i) {
					m_alloc.construct(&new_data[i + (holeSize * (i >= idx))], m_data[i]);
					m_alloc.destroy(&m_data[i]);
				}

				m_alloc.deallocate(m_data, m_capacity);

				m_capacity = new_capacity;
				m_data = new_data;
			} else {
				for (size_type i = m_size; i > idx; --i) {
					if ((i + holeSize) <= m_size) {
						m_data[(i - 1) + holeSize] = m_data[(i - 1)];
					} else {
						m_alloc.construct(&m_data[(i - 1) + holeSize], m_data[(i - 1)]);
					}
				}

				for (size_type i = 0; i < holeSize; ++i) {
					if ((i + idx) >= m_size) {
						break;
					}
					m_alloc.destroy(&m_data[idx + i]);
				}
			}
			m_size += holeSize;
		}

		void move_data(_Tp *new_data) {
			for (size_type i = 0; i < m_size; ++i) {
				m_alloc.construct(&new_data[i], m_data[i]);
				m_alloc.destroy(&m_data[i]);
			}
		}

		/**
		 * @return true if m_data was reallocated (so it contains only stall memory);
		 * 	false m_data was reused (so it still contains valid copies)
		 */
		size_type ensure_replacing_allocation(size_type count) {
			if (m_capacity < count) {
				free_data();
				m_capacity = calc_capacity(count);
				m_data = m_alloc.allocate(m_capacity);
				m_size = count;
				return 0;
			}

			for (size_type i = count; i < m_size; ++i) {
				m_alloc.destroy(&(m_data[i]));
			}

			size_type retValue = (m_size < count) ? m_size : count;
			m_size = count;
			return retValue;
		}
	};

	template<typename _Tp, typename _Alloc>
	bool operator==(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		if (v0.size() != v1.size()) {
			return false;
		}

		return ft::equal(v0.begin(), v0.end(), v1.begin());
	}

	template<typename _Tp, typename _Alloc>
	bool operator!=(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		return !(v0 == v1);
	}

	template<typename _Tp, typename _Alloc>
	bool operator<(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		return ft::lexicographical_compare(v0.begin(), v0.end(), v1.begin(), v1.end());
	}

	template<typename _Tp, typename _Alloc>
	bool operator<=(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		return !(v1 < v0);
	}

	template<typename _Tp, typename _Alloc>
	bool operator>(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		return v1 < v0;
	}

	template<typename _Tp, typename _Alloc>
	bool operator>=(const vector<_Tp, _Alloc> &v0, const vector<_Tp, _Alloc> &v1) {
		return !(v0 < v1);
	}

	template<typename _Tp, typename _Alloc>
	void swap(vector<_Tp, _Alloc> &v0, vector<_Tp, _Alloc> &v1) {
		v0.swap(v1);
	}
} // namespace ft
