#pragma once

#include <cstddef>

namespace ft {
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag: public input_iterator_tag {};
	struct bidirectional_iterator_tag: public forward_iterator_tag {};
	struct random_access_iterator_tag: public bidirectional_iterator_tag {};
	struct contiguous_iterator_tag: public random_access_iterator_tag {};

	template<typename _Iterator>
	struct iterators_traits {
		typedef typename _Iterator::difference_type difference_type;
		typedef typename _Iterator::value_type value_type;
		typedef typename _Iterator::pointer pointer;
		typedef typename _Iterator::reference reference;
		typedef typename _Iterator::iterator_category iterator_category;
	};

	template<typename _Tp>
	struct iterators_traits<_Tp *> {
		typedef std::ptrdiff_t difference_type;
		typedef _Tp value_type;
		typedef _Tp *pointer;
		typedef _Tp &reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template<typename _Tp>
	struct iterators_traits<const _Tp *> {
		typedef std::ptrdiff_t difference_type;
		typedef _Tp value_type;
		typedef _Tp *pointer;
		typedef _Tp &reference;
		typedef random_access_iterator_tag iterator_category;
	};
} // namespace ft
