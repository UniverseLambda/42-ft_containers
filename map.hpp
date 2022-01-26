#pragma once

#include "bits/bst.hpp"

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

			inline const _Key &operator()(const ft::pair<const _Key, _Value> &d) const {
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

		class value_compare: public std::binary_function<value_type, value_type, bool> {
			friend class map;
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
		typedef const __clsaad_impl::bst_wrapper<value_type, value_compare, allocator_type, key_type, key_extractor, key_compare> const_tree_type;

	public:
		typedef __clsaad_impl::bst_iterator<typename tree_type::node_type, value_type> iterator;
		typedef __clsaad_impl::bst_iterator<const typename const_tree_type::node_type, const value_type> const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

	private:
		tree_type bst;

	public:
		map():
			bst(value_compare(_KeyLess()), _Allocator(), key_extractor(), _KeyLess()) {}

		explicit map(const _KeyLess &key_less, const _Allocator &alloc = _Allocator()):
			bst(value_compare(_KeyLess()), alloc, key_extractor(), key_less) {}

		template<typename _InputIt>
		map(_InputIt first, _InputIt last, const _KeyLess &key_less = _KeyLess(), const _Allocator &alloc = _Allocator()):
			bst(value_compare(key_less), alloc, key_extractor(), key_less) {
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
			return bst.find_node(key).data->second;
		}

		const _Tp &at(const _Key &key) const {
			return bst.find_node(key).data->second;
		}

		_Tp &operator[](const _Key &key) {
			ft::pair<typename tree_type::node_type *, bool> result = bst.insert(ft::make_pair(key, _Tp()));

			return result.first->data->second;
		}

		iterator begin() {
			return iterator(bst.min_node());
		}

		const_iterator begin() const {
			return const_iterator(bst.min_node());
		}

		iterator end() {
			return iterator(&bst.get_anchor());
		}

		const_iterator end() const {
			return const_iterator(&bst.get_anchor());
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
			return size() == 0;
		}

		size_type size() const {
			return bst.get_element_count();
		}

		size_type max_size() const {
			return std::numeric_limits<difference_type>::max() / (sizeof(*this) + sizeof(typename tree_type::node_type) + sizeof(value_type));
		}

		void clear() {
			bst.clear();
		}

		ft::pair<iterator, bool> insert(const value_type &value) {
			ft::pair<typename tree_type::node_type *, bool> result = bst.insert(value);

			return ft::pair<iterator, bool>(iterator(result.first), result.second);
		}

		// TODO: use hint...
		iterator insert(iterator, const value_type &value) {
			ft::pair<typename tree_type::node_type *, bool> result = bst.insert(value);

			return iterator(result.first);
		}

		template<typename _InputIt>
		void insert(_InputIt first, _InputIt last) {
			for (; first != last; ++first) {
				insert(*first);
			}
		}

		void erase(iterator it) {
			bst.erase(it.__get_node());
		}

		void erase(iterator first, iterator last) {
			while (first != last) {
				iterator tmp = first++;

				erase(tmp);
			}
		}

		size_type erase(const _Key &key) {
			return !!(bst.erase(key));
		}

		void swap(map &other) {
			bst.swap(other.bst);
		}

		size_type count(const _Key &key) const {
			try {
				bst.find_node(key);

				return 1;
			} catch (std::out_of_range &ofr) {
				(void)ofr;
			}
			return 0;
		}

		iterator find(const _Key &key) {
			try {
				typename tree_type::node_type &node = bst.find_node(key);

				return iterator(&node);
			} catch (std::out_of_range &ofr) {
				(void)ofr;
			}
			return end();
		}

		const_iterator find(const _Key &key) const {
			try {
				const typename tree_type::node_type &node = bst.find_node(key);

				return const_iterator(&node);
			} catch (std::out_of_range &ofr) {
				(void)ofr;
			}
			return end();
		}

		iterator lower_bound(const _Key &key) {
			typename tree_type::node_type *node = bst.lower_bound(key);

			if (node == NULL)
				return end();
			return iterator(node);
		}

		const_iterator lower_bound(const _Key &key) const {
			typename tree_type::node_type *node = bst.lower_bound(key);

			if (node == NULL)
				return end();
			return const_iterator(node);
		}

		iterator upper_bound(const _Key &key) {
			typename tree_type::node_type *node = bst.lower_bound(key);

			if (node == NULL)
				return end();

			if (__clsaad_impl::is_equivalent(bst.get_key_less(), node->data->first, key)) {
				return ++(iterator(node));
			} else {
				return iterator(node);
			}
		}

		const_iterator upper_bound(const _Key &key) const {
			typename tree_type::node_type *node = bst.lower_bound(key);

			if (node == NULL)
				return end();

			if (__clsaad_impl::is_equivalent(bst.get_key_less(), node->data->first, key)) {
				return ++(const_iterator(node));
			} else {
				return const_iterator(node);
			}
		}

		ft::pair<iterator, iterator> equal_range(const _Key &key) {
			typename tree_type::node_type *node = bst.lower_bound(key);

			if (node == NULL)
				return ft::pair<iterator, iterator>(end(), end());

			if (__clsaad_impl::is_equivalent(bst.get_key_less(), node->data->first, key)) {
				return ft::pair<iterator, iterator>(iterator(node), ++(iterator(node)));
			} else {
				return ft::pair<iterator, iterator>(iterator(node), iterator(node));
			}
		}

		ft::pair<const_iterator, const_iterator> equal_range(const _Key &key) const {
			typename tree_type::node_type *node = bst.lower_bound(key);

			if (node == NULL)
				return ft::pair<const_iterator, const_iterator>(end(), end());

			if (__clsaad_impl::is_equivalent(bst.get_key_less(), node->data->first, key)) {
				return ft::pair<const_iterator, const_iterator>(const_iterator(node), ++(const_iterator(node)));
			} else {
				return ft::pair<const_iterator, const_iterator>(const_iterator(node), const_iterator(node));
			}
		}

		key_compare key_comp() const {
			return bst.get_key_less();
		}

		value_compare value_comp() const {
			return value_compare(key_comp());
		}
	};

	template<typename _Key, typename _T, typename _Compare, typename _Alloc>
	bool operator==(const ft::map<_Key, _T, _Compare, _Alloc> &lhs, const ft::map<_Key, _T, _Compare, _Alloc> &rhs) {
		if (lhs.size() != rhs.size())
			return false;
		return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
	};

	template<typename _Key, typename _T, typename _Compare, typename _Alloc>
	bool operator<(const ft::map<_Key, _T, _Compare, _Alloc> &lhs, const ft::map<_Key, _T, _Compare, _Alloc> &rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename _Key, typename _T, typename _Compare, typename _Alloc>
	bool operator!=(const ft::map<_Key, _T, _Compare, _Alloc> &lhs, const ft::map<_Key, _T, _Compare, _Alloc> &rhs) {
		return !(lhs == rhs);
	};

	template<typename _Key, typename _T, typename _Compare, typename _Alloc>
	bool operator<=(const ft::map<_Key, _T, _Compare, _Alloc> &lhs, const ft::map<_Key, _T, _Compare, _Alloc> &rhs) {
		return !(rhs < lhs);
	}

	template<typename _Key, typename _T, typename _Compare, typename _Alloc>
	bool operator>(const ft::map<_Key, _T, _Compare, _Alloc> &lhs, const ft::map<_Key, _T, _Compare, _Alloc> &rhs) {
		return rhs < lhs;
	}

	template<typename _Key, typename _T, typename _Compare, typename _Alloc>
	bool operator>=(const ft::map<_Key, _T, _Compare, _Alloc> &lhs, const ft::map<_Key, _T, _Compare, _Alloc> &rhs) {
		return !(lhs < rhs);
	}

	template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
	void swap(ft::map<_Key, _Tp, _Compare, _Alloc> &lhs, ft::map<_Key, _Tp, _Compare, _Alloc> &rhs) {
		lhs.swap(rhs);
	}
} // namespace ft
