#pragma once

#define __FT_BITS_INC
#include "bits/bst.hpp"
#undef __FT_BITS_INC

#include <functional>
#include <memory>
#include "utility.hpp"

namespace ft {
	template<typename _Key, typename _Tp, typename _Compare = std::less<_Key>, typename _Allocator = std::allocator<ft::pair<const _Key, _Tp> > >
	class map {
	public:
		typedef _Key key_type;
		typedef _Tp mapped_type;
		typedef ft::pair<const _Key, _Tp> value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef _Compare key_compare;
		typedef _Allocator allocator_type;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef value_type *pointer;
		typedef const value_type *const_pointer;
		// typedef iterator;
		// typedef const_iterator;
		// typedef reverse_iterator;
		// typedef const_reverse_iterator;

	private:
		typedef __clsaad_impl::BSTNode<_Key, _Tp, _Compare, _Allocator> node_type;

		key_compare comp;
		allocator_type alloc;
		node_type *root;

	public:
		map(): root(NULL) {}

		explicit map(const _Compare &comp, const _Allocator &alloc = _Allocator()):
			root(NULL),
			comp(comp),
			alloc(alloc) {}

		template<typename _InputIt>
		map(_InputIt first, _InputIt last, const _Compare& comp = _Compare(), const _Allocator& alloc = _Allocator()):
			root(NULL),
			comp(comp),
			alloc(alloc) {
			insert(first, last);
		}

		map(const map &other):
			comp(other.comp),
			alloc(other.alloc) {
			root = (other.root == NULL) ? NULL : new node_type(*other.root, &root);
		}

		~map() {
			delete root;
		}

		map &operator=(const map &rhs) {
			if (root != NULL)
				delete root;

			comp = rhs.comp;
			alloc = rhs.alloc;
			root = (rhs.root == NULL) ? NULL : new node_type(*rhs.root, &root);

			return (*this);
		}

		allocator_type get_allocator() const {
			return alloc;
		}

		_Tp &at(const _Key &key) {
			if (root == NULL)
				std::__throw_out_of_range("key not found");
			return root->find_value(key);
		}

		const _Tp &at(const _Key &key) const {
			if (root == NULL)
				std::__throw_out_of_range("key not found");
			return root->find_value(key);
		}

		_Tp &operator[](const _Key &key) {
			if (root == NULL) {
				return insert(ft::make_pair(key, _Tp())).first->second;
			}
			return root->find_or_create(key);
		}


		class value_compare: public std::binary_function<value_type, value_type, bool> {
		protected:
			_Compare comp;

			value_compare(_Compare c): comp(c) {}

		public:
			bool operator()(const value_type &lhs, const value_type &rhs) const {
				return comp(lhs.first, rhs.first);
			}
		};
	};
} // namespace ft
