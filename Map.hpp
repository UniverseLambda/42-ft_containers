#pragma once

#define __FT_BITS_INC
#include "bits/bst.hpp"
#undef __FT_BITS_INC

#include <functional>
#include <memory>
#include "utility.hpp"

namespace ft {
	namespace __clsaad_impl {
		template<typename _Key, typename _Value>
		struct pair_key_extractor {
			pair_key_extractor() {}
			pair_key_extractor(const pair_key_extractor &) {}
			~pair_key_extractor() {}
			pair_key_extractor &operator=(const pair_key_extractor &) { return (*this); }

			inline _Key &operator()(ft::pair<const _Key, _Value> &d) const {
				return d.first;
			}
		};
	} // namespace __clsaad_impl
	

	template<typename _Key, typename _Tp, typename _KeyLess = std::less<_Key>, typename _Allocator = std::allocator<ft::pair<const _Key, _Tp> > >
	class map {
	public:
		typedef _Key key_type;
		typedef _Tp mapped_type;
		typedef ft::pair<const _Key, _Tp> value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef _KeyLess key_compare;
		typedef _Allocator allocator_type;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		// typedef iterator;
		// typedef const_iterator;
		// typedef reverse_iterator;
		// typedef const_reverse_iterator;

		class value_compare: public std::binary_function<value_type, value_type, bool> {
		protected:
			_KeyLess comp;

			value_compare(_KeyLess c): comp(c) {}

		public:
			bool operator()(const value_type &lhs, const value_type &rhs) const {
				return comp(lhs.first, rhs.first);
			}
		};

	private:
		typedef __clsaad_impl::pair_key_extractor<_Key, _Tp> key_extractor;
		typedef __clsaad_impl::bst_wrapper<value_type, value_compare, allocator_type, key_type, key_extractor, key_compare> tree_type;

		tree_type bst;

	public:
		map():
			bst(value_compare(), _Allocator(), key_extractor(), _KeyLess()) {}

		explicit map(const _KeyLess &key_less, const _Allocator &alloc = _Allocator()):
			bst(value_compare(), alloc, key_extractor(), key_less) {}

		template<typename _InputIt>
		map(_InputIt first, _InputIt last, const _KeyLess &key_less = _KeyLess(), const _Allocator &alloc = _Allocator()):
			bst(value_compare(), alloc, key_extractor(), key_less) {
			insert(first, last);
		}

		map(const map &other):
			bst(other.bst) {}

		~map() {}

		map &operator=(const map &rhs) {
			bst = rhs.bst;

			return (*this);
		}

		allocator_type get_allocator() const {
			return bst.get_allocator();
		}

		_Tp &at(const _Key &key) {
			return bst.find_node(key);
		}

		const _Tp &at(const _Key &key) const {
			return bst.find_node(key);
		}

		_Tp &operator[](const _Key &key) {
			ft::pair<typename tree_type::node_type *, bool> result = bst.insert(ft::make_pair(key, _Tp()));

			return *(result.first->data->first);
		}
	};
} // namespace ft
