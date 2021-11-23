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
	};
} // namespace ft
