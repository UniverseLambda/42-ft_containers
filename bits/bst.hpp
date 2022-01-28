#pragma once

// #ifndef __FT_BITS_INC
// # error "You included a bits header"
// #endif

#include <functional>
#include <memory>

#include "../utility.hpp"
#include "../iterator.hpp"

namespace ft {
	namespace __clsaad_impl {
		enum bst_node_color {
			BLACK,
			RED
		};

		template<typename _Data>
		struct default_key_extractor {
			default_key_extractor() {}
			default_key_extractor(const default_key_extractor &) {}
			~default_key_extractor() {}
			default_key_extractor &operator=(const default_key_extractor &) { return (*this); }

			_Data &operator()(_Data &d) const {
				return d;
			}
		};

		template<typename _Tp, typename _Less>
		inline bool is_equivalent(_Less &less, const _Tp &val0, const _Tp &val1) {
			return (!less(val0, val1) && !less(val1, val0));
		}

		template<typename _Tp>
		inline void try_assign_ptr(_Tp *target, _Tp value) {
			if (target != NULL) {
				*target = value;
			}
		}

		template<typename _Data, typename _Allocator>
		inline _Data *bst_allocate(_Allocator &alloc, const _Data &data) {
			_Data *new_data = alloc.allocate(1);
			alloc.construct(new_data, data);

			return new_data;
		}

		template<typename _Data, typename _Allocator>
		inline _Data *bst_copy_data(_Allocator &alloc, _Data *data) {
			if (data == NULL) {
				return NULL;
			}

			_Data *new_data = alloc.allocate(1);
			alloc.construct(new_data, *data);
			return new_data;
		}

		template<typename _Data, typename _Allocator>
		inline void bst_delete_data(_Allocator &alloc, _Data *data) {
			if (data == NULL)
				return;
			alloc.destroy(data);
			alloc.deallocate(data, 1);
		}

		template<typename _Node>
		inline _Node *move_to_dyn_storage(const _Node &node) {
			std::allocator<_Node> alloc;
			_Node *node_ptr;

			node_ptr = alloc.allocate(1);
			alloc.construct(node_ptr, node);

			return node_ptr;
		}

		template<typename _Node>
		inline void destroy_from_dyn_storage(_Node *node) {
			std::allocator<_Node> alloc;

			alloc.destroy(node);
			alloc.deallocate(node, 1);
		}

		template<typename _Tree, typename _Data>
		struct bst_node {
			_Tree *tree;

			bst_node *parent;

			_Data *data;

			bst_node_color node_color;

			bst_node *left_node;
			bst_node *right_node;

			bool temporary;

			bst_node(_Tree *tree, bst_node *parent, _Data *data, bst_node_color color):
				tree(tree),
				parent(parent),
				data(data),
				node_color(color),
				left_node(NULL),
				right_node(NULL),
				temporary(true) {
				if (!parent) {
					node_color = BLACK;
				}
			}

			bst_node(_Tree *tree, const bst_node &other):
				tree(tree),
				parent(NULL),
				data(bst_copy_data(tree->get_allocator(), other.data)),
				node_color(other.node_color),
				left_node(NULL),
				right_node(NULL),
				temporary(true) {

				if (other.left_node != NULL && other.left_node->data != NULL) {
					left_node = move_to_dyn_storage(bst_node(tree, *(other.left_node)));
					left_node->parent = this;
				}

				if (other.right_node != NULL && other.right_node->data != NULL) {
					right_node = move_to_dyn_storage(bst_node(tree, *(other.right_node)));
					right_node->parent = this;
				}

			}

			bst_node(const bst_node &cpy):
				tree(cpy.tree),
				parent(cpy.parent),
				data(cpy.data),
				node_color(cpy.node_color),
				left_node(cpy.left_node),
				right_node(cpy.right_node),
				temporary(false) {

				if (left_node != NULL) {
					left_node->parent = this;
				}

				if (right_node != NULL) {
					right_node->parent = this;
				}
			}

			~bst_node() {
				if (!temporary) {
					if (right_node && right_node->data != NULL)
						destroy_from_dyn_storage(right_node);

					if (left_node && left_node->data != NULL)
						destroy_from_dyn_storage(left_node);

					bst_delete_data(tree->get_allocator(), data);
				}
			}

		private:
			bst_node &operator=(const bst_node &) {
				this->you_should_definitely_not_use_this_you_filthy_bad_boy();
				return (*this);
			}

		public:
			void insertion_rebalance_tree() {
				bst_node *grandparent;
				bst_node *uncle;

				if (*(tree->get_root()) == this || parent == NULL) {
					node_color = BLACK;
					return;
				}

				if (parent->node_color == BLACK) {
					return;
				}

				grandparent = parent->parent;
				uncle = grandparent->sibling(parent);

				if (uncle != NULL && uncle->node_color == RED) {
					parent->node_color = BLACK;
					uncle->node_color = BLACK;
					grandparent->node_color = RED;
					grandparent->insertion_rebalance_tree();
				} else {
					if (grandparent->left_node == parent && parent->left_node == this) {
						rebalance_ll(grandparent, parent);
					} else if (grandparent->left_node == parent && parent->right_node == this) {
						rebalance_lr(grandparent, parent);
					} else if (grandparent->right_node == parent && parent->right_node == this) {
						rebalance_rr(grandparent, parent);
					} else if (grandparent->right_node == parent && parent->left_node == this) {
						rebalance_rl(grandparent, parent);
					} else {
						throw std::runtime_error("WHAT????");
					}
				}
			}

			void right_rotate() {
				bst_node *pivot;

				if (!left_node) {
					return;
				}

				pivot = left_node;

				left_node = pivot->right_node;

				if (left_node)
					left_node->parent = this;

				pivot->right_node = this;

				pivot->parent = parent;

				if (parent) {
					parent->node_storage(this) = pivot;
				} else {
					*(tree->get_root()) = pivot;
				}

				parent = pivot;
			}

			void left_rotate() {
				bst_node *pivot;

				if (!right_node) {
					return;
				}

				pivot = right_node;

				right_node = pivot->left_node;
				if (right_node)
					right_node->parent = this;

				pivot->left_node = this;
				pivot->parent = parent;

				if (parent) {
					parent->node_storage(this) = pivot;
				} else {
					*(tree->get_root()) = pivot;
				}

				parent = pivot;
			}

			bst_node *min_node() {
				if (left_node == NULL) {
					return this;
				}

				bst_node *node = left_node;

				while (node->left_node != NULL && node->left_node->data != NULL) {
					node = node->left_node;
				}

				return node;
			}

			inline bst_node *sibling(bst_node *node) {
				return (node == left_node) ? right_node : ((node == right_node) ? left_node : NULL);
			}

			inline bst_node *&node_storage(bst_node *node) {
				if (node == left_node) {
					return left_node;
				}

				if (node == right_node) {
					return right_node;
				}

				throw std::runtime_error("internal error");
			}

		private:
			void rebalance_ll(bst_node *grandparent, bst_node *parent) {
				bst_node_color tmp;

				grandparent->right_rotate();

				tmp = parent->node_color;
				parent->node_color = grandparent->node_color;
				grandparent->node_color = tmp;
			}

			void rebalance_lr(bst_node *grandparent, bst_node *parent) {
				bst_node_color tmp;

				parent->left_rotate();
				grandparent->right_rotate();

				tmp = node_color;
				node_color = grandparent->node_color;
				grandparent->node_color = tmp;
			}

			void rebalance_rr(bst_node *grandparent, bst_node *parent) {
				bst_node_color tmp;

				grandparent->left_rotate();

				tmp = parent->node_color;
				parent->node_color = grandparent->node_color;
				grandparent->node_color = tmp;
			}

			void rebalance_rl(bst_node *grandparent, bst_node *parent) {
				bst_node_color tmp;

				parent->right_rotate();
				grandparent->left_rotate();

				tmp = node_color;
				node_color = grandparent->node_color;
				grandparent->node_color = tmp;
			}

		public:
			static void set_color(bst_node *node, bst_node_color color) {
				if (node != NULL) {
					node->node_color = color;
				}
			}

			static bst_node_color get_color(bst_node *node) {
				return (node != NULL) ? node->node_color : BLACK;
			}

			static bst_node *get_left_node(bst_node *node) {
				return (node != NULL) ? node->left_node : NULL;
			}

			static bst_node *get_right_node(bst_node *node) {
				return (node != NULL) ? node->right_node : NULL;
			}
		};

		template<typename _Wrapper>
		struct bst_wrapper_anchor_guard {
			_Wrapper &wrapper;

			bst_wrapper_anchor_guard(_Wrapper &wrapper): wrapper(wrapper) {
				wrapper.remove_anchor();
			}

			~bst_wrapper_anchor_guard() {
				wrapper.set_anchor();
			}

		private:
			bst_wrapper_anchor_guard(const bst_wrapper_anchor_guard &) {}
			bst_wrapper_anchor_guard& operator=(const bst_wrapper_anchor_guard &) {}

		};

		template<typename _Data, typename _Less, typename _Allocator = std::allocator<_Data>, typename _Key = _Data, typename _KeyExtractor = default_key_extractor<_Key>, typename _KeyLess = _Less>
		class bst_wrapper {
		public:
			typedef bst_node<bst_wrapper, _Data> node_type;
			typedef bst_wrapper_anchor_guard<bst_wrapper> anchor_guard_type;

		private:
			_Less less;
			_Allocator alloc;
			_KeyExtractor key_extractor;
			_KeyLess key_less;

			node_type *root;
			node_type anchor;

			std::size_t element_cout;
			std::allocator<node_type> node_allocator;

		public:
			bst_wrapper(const _Less &less = _Less(), const _Allocator &alloc = _Allocator(), const _KeyExtractor &key_extractor = _KeyExtractor(), const _KeyLess &key_less = _KeyLess()):
				less(less),
				alloc(alloc),
				key_extractor(key_extractor),
				key_less(key_less),
				root(NULL),
				anchor(this, NULL, NULL, BLACK),
				element_cout(0)
				{}

			bst_wrapper(const bst_wrapper &other):
				less(other.less),
				alloc(other.alloc),
				key_extractor(other.key_extractor),
				key_less(other.key_less),
				root(NULL),
				anchor(this, NULL, NULL, BLACK),
				element_cout(other.element_cout) {

				if (other.root != NULL) {
					root = move_to_dyn_storage(node_type(this, *other.root));
				}

				set_anchor();
			}

			~bst_wrapper() {
				anchor.right_node = NULL;
				anchor.left_node = NULL;
				if (root != NULL) {
					destroy_from_dyn_storage(root);
				}
			}

			bst_wrapper &operator=(const bst_wrapper &rhs) {
				if (root != NULL)
					destroy_from_dyn_storage(root);

				if (rhs.root == NULL) {
					root = NULL;
				} else {
					root = move_to_dyn_storage(node_type(*rhs.root, *this));
				}

				return (*this);
			}

			void set_anchor() {
				if (root == NULL) {
					anchor.left_node = NULL;
					anchor.right_node = NULL;
					return;
				}

				node_type *left_node = root;
				node_type *right_node = root;

				while (left_node->left_node != NULL || right_node->right_node != NULL) {
					if (left_node->left_node != NULL) {
						left_node = left_node->left_node;
					}

					if (right_node->right_node != NULL) {
						right_node = right_node->right_node;
					}
				}

				// Yep. They are reversed. As decrementing bst_iterator(anchor) need to go back to max_node
				anchor.right_node = left_node;
				anchor.left_node = right_node;

				left_node->left_node = &anchor;
				right_node->right_node = &anchor;
			}

			void remove_anchor() {
				if (anchor.left_node)
					anchor.left_node->right_node = NULL;
				if (anchor.right_node)
					anchor.right_node->left_node = NULL;
				anchor.left_node = NULL;
				anchor.right_node = NULL;
			}

			ft::pair<node_type *, bool> insert(const _Data &data) {
				if (root == NULL) {
					root = move_to_dyn_storage(node_type(this, NULL, bst_allocate(alloc, data), BLACK));
					set_anchor();
					++element_cout;
					return ft::make_pair(root, true);
				}

				node_type *parent;
				bool is_less;

				node_type *node = retrieve_node(data, parent, is_less);

				if (node != NULL) {
					return ft::make_pair(node, false);
				}

				anchor_guard_type guard(*this);
				node = move_to_dyn_storage(node_type(this, parent, bst_allocate(alloc, data), RED));

				if (is_less) {
					parent->left_node = node;
				} else {
					parent->right_node = node;
				}

				node->insertion_rebalance_tree();
				++element_cout;
				return ft::make_pair(node, true);
			}

			node_type &find_node(const _Key &key) {
				node_type *result = retrieve_node_key(key);

				if (result == NULL)
					throw std::out_of_range("key not found");

				return *result;
			}

			const node_type &find_node(const _Key &key) const {
				node_type *result = retrieve_node_key(key);

				if (result == NULL)
					throw std::out_of_range("key not found");

				return *result;
			}

			bool erase(const _Key &key) {
				node_type *node = retrieve_node_key(key);

				if (node == NULL) {
					return false;
				}

				erase(node);
				return true;
			}

			void erase(node_type *node) {
				anchor_guard_type guard(*this);

				int child_count = !!(node->left_node) + !!(node->right_node);

				if (child_count == 2) {
					swap_places(node, node->right_node->min_node());
					child_count = !!(node->left_node) + !!(node->right_node);
				}

				node_type *replacement = (node->left_node != NULL) ? node->left_node : node->right_node;

				if (replacement != NULL) {
					replacement->parent = node->parent;
				}

				if (root == node) {
					root = replacement;
					node_type::set_color(replacement, BLACK);
				} else {
					node->parent->node_storage(node) = replacement;

					if (node->node_color == RED || node_type::get_color(replacement) == RED) {
						node_type::set_color(replacement, BLACK);
					} else {
						propagate_double_black(replacement, node);
					}
				}

				// Setting them to NULL to prevent them from being deleted
				node->left_node = NULL;
				node->right_node = NULL;
				destroy_from_dyn_storage(node);
				--element_cout;
			}

			_Allocator &get_allocator() {
				return alloc;
			}

			const _Allocator &get_allocator() const {
				return alloc;
			}

			_Less &get_less() {
				return less;
			}

			const _Less &get_less() const {
				return less;
			}

			_KeyLess &get_key_less() {
				return key_less;
			}

			const _KeyLess &get_key_less() const {
				return key_less;
			}

			node_type **get_root() {
				return &root;
			}

			const node_type **get_root() const {
				return &root;
			}

			void clear() {
				anchor_guard_type guard(*this);

				if (root != NULL) {
					destroy_from_dyn_storage(root);
				}
				root = NULL;
				element_cout = 0;
			}

			node_type *retrieve_node_key(const _Key &key, node_type **parent = NULL, bool *is_less = NULL) const {
				node_type *current_node;
				node_type *next_node = root;
				bool current_is_less = false;

				if (root == NULL) {
					try_assign_ptr<node_type *>(parent, NULL);
					try_assign_ptr(is_less, current_is_less);
					return NULL;
				}

				while (next_node != NULL && next_node->data != NULL) {
					current_node = next_node;

					if (is_equivalent(key_less, key, key_extractor(*(current_node->data)))) {
						try_assign_ptr(parent, current_node->parent);
						try_assign_ptr(is_less, current_is_less);
						return current_node;
					}

					if (key_less(key, key_extractor(*(current_node->data)))) {
						next_node = current_node->left_node;
						current_is_less = true;
					} else {
						next_node = current_node->right_node;
						current_is_less = false;
					}
				}

				try_assign_ptr(parent, current_node);
				try_assign_ptr(is_less, current_is_less);

				return NULL;
			}

			inline node_type *retrieve_node_key(const _Key &key, node_type *&parent) const { return retrieve_node_key(key, &parent); }
			inline node_type *retrieve_node_key(const _Key &key, bool &is_less) const { return retrieve_node_key(key, NULL, &is_less); }
			inline node_type *retrieve_node_key(const _Key &key, node_type *&parent, bool &is_less) const { return retrieve_node_key(key, &parent, &is_less); }

			node_type *retrieve_node(const _Data &data, node_type **parent = NULL, bool *is_less = NULL) const {
				node_type *current_node;
				node_type *next_node = root;
				bool current_is_less = false;

				if (root == NULL) {
					try_assign_ptr<node_type *>(parent, NULL);
					try_assign_ptr(is_less, current_is_less);
					return NULL;
				}

				while (next_node != NULL && next_node->data != NULL) {
					current_node = next_node;

					if (is_equivalent(less, data, *(current_node->data))) {
						try_assign_ptr(parent, current_node->parent);
						try_assign_ptr(is_less, current_is_less);
						return current_node;
					}

					if (less(data, *(current_node->data))) {
						next_node = current_node->left_node;
						current_is_less = true;
					} else {
						next_node = current_node->right_node;
						current_is_less = false;
					}
				}

				try_assign_ptr(parent, current_node);
				try_assign_ptr(is_less, current_is_less);

				return NULL;
			}

			inline node_type *retrieve_node(const _Data &data, node_type *&parent) const { return retrieve_node(data, &parent); }
			inline node_type *retrieve_node(const _Data &data, bool &is_less) const { return retrieve_node(data, NULL, &is_less); }
			inline node_type *retrieve_node(const _Data &data, node_type *&parent, bool &is_less) const { return retrieve_node(data, &parent, &is_less); }

			void swap_places(node_type *n0, node_type *n1) {
				std::swap(n0->left_node, n1->left_node);
				std::swap(n0->right_node, n1->right_node);
				std::swap(n0->parent, n1->parent);
				std::swap(n0->node_color, n1->node_color);

				if (n0->parent == NULL) {
					root = n0;
				} else {
					if (n0->parent == n0) {
						n0->parent = n1;
						((n1->left_node == n1) ? n1->left_node : n1->right_node) = n0;
					} else {
						n0->parent->node_storage(n1) = n0;
					}
				}

				if (n1->parent == NULL) {
					root = n1;
				} else {
					if (n1->parent == n1) {
						n1->parent = n0;
						((n0->left_node == n0) ? n0->left_node : n0->right_node) = n1;
					} else {
						n1->parent->node_storage(n0) = n1;
					}
				}

				if (n0->left_node != NULL) n0->left_node->parent = n0;
				if (n0->right_node != NULL) n0->right_node->parent = n0;
				if (n1->left_node != NULL) n1->left_node->parent = n1;
				if (n1->right_node != NULL) n1->right_node->parent = n1;
			}

			const node_type *min_node() const {
				return (root == NULL) ? end_node() : anchor.right_node;
			}

			node_type *min_node() {
				return (root == NULL) ? end_node() : anchor.right_node;
			}

			const node_type *max_node() const {
				return (root == NULL) ? end_node() : anchor.left_node;
			}

			node_type *max_node() {
				return (root == NULL) ? end_node() : anchor.left_node;
			}

			node_type *end_node() {
				return &anchor;
			}

			const node_type *end_node() const {
				return &anchor;
			}

			std::size_t get_element_count() const {
				return element_cout;
			}

			node_type &get_anchor() {
				return anchor;
			}

			const node_type &get_anchor() const {
				return anchor;
			}

			// Before C++11, whether the allocator is swapped or not is undefined... Even though stateful allocators may fail after this call,
			// the Allocator requirement does not require Swappable... So we do not swap alloc so it will compile for every Allocators.
			void swap(bst_wrapper &other) {
				anchor_guard_type guard0(*this);
				anchor_guard_type guard1(other);

				std::swap(key_less, other.key_less);
				std::swap(element_cout, other.element_cout);
				std::swap(root, other.root);

				if (root != NULL) propagate_set_tree(root);
				if (other.root != NULL) other.propagate_set_tree(other.root);
			}

			node_type *lower_bound(const _Key &key) const {
				node_type *node = root;

				while (node != NULL && node->data != NULL) {
					const _Key &curr = key_extractor(*node->data);

					if (is_equivalent(key_less, key, curr)) {
						break;
					}

					if (key_less(curr, key)) {
						node = node->right_node;
					} else {
						if (node->left_node == NULL) {
							break;
						}
						const _Key &left = key_extractor(*(node->left_node->data));

						if (key_less(left, key)) {
							break;
						}
						node = node->left_node;
					}
				}

				if (node != NULL && node->data == NULL)
					return NULL;
				return node;
			}

		private:
			void propagate_double_black(node_type *target, node_type *replaced = NULL) {
				node_type *node_this = (target == NULL) ? replaced : target;

				if (node_this->parent == NULL) {
					// THIS IS THE ROOT, SO DON'T DO ANYTHING ASIDE OF RECOLORING IT
					node_type::set_color(target, BLACK);
					return;
				}

				node_type *sibling = node_this->parent->sibling(target);

				if (node_type::get_color(sibling) == RED) {
					if (sibling == node_this->parent->left_node) {
						node_this->parent->left_rotate();
						std::swap(node_this->parent->node_color, sibling->node_color);

					} else {
						node_this->parent->right_rotate();
						std::swap(node_this->parent->node_color, sibling->node_color);
					}

					sibling = node_this->parent->sibling(node_this);
				}

				node_type *sibling_left = node_type::get_left_node(sibling);
				node_type *sibling_right = node_type::get_right_node(sibling);

				if (node_type::get_color(sibling) == BLACK) {
					node_type *sibling_red = NULL;

					if (node_type::get_color(sibling_right) == RED) {
						sibling_red = sibling_right;
					} else if (node_type::get_color(sibling_left) == RED) {
						sibling_red = sibling_left;
					}

					if (sibling_red != NULL) {
						if (sibling == node_this->parent->right_node) {
							if (sibling_red == sibling_right) {
								node_this->parent->right_rotate();
								sibling_right->node_color = BLACK;
							} else {
								sibling->left_rotate();
								std::swap(sibling->node_color, sibling_red->node_color);

								node_this->parent->right_rotate();
								sibling->node_color = BLACK;
							}
						} else {
							if (sibling_red == sibling_left) {
								node_this->parent->left_rotate();
								sibling_left->node_color = BLACK;
							} else {
								sibling->right_rotate();
								std::swap(sibling->node_color, sibling_red->node_color);

								node_this->parent->left_rotate();
								sibling->node_color = BLACK;
							}
						}
					} else {
						node_type::set_color(sibling, RED);

						if (node_this->parent->node_color == BLACK) {
							propagate_double_black(node_this->parent);
						} else {
							node_this->parent->node_color = BLACK;
						}
					}
				}
			}

			void propagate_set_tree(node_type *node) {
				node->tree = this;

				if (node->left_node) propagate_set_tree(node->left_node);
				if (node->right_node) propagate_set_tree(node->right_node);
			}
		};

		template<typename _Node, typename _Tp>
		class bst_iterator {
		public:
			typedef _Tp value_type;
			typedef std::ptrdiff_t difference_type;
			typedef _Tp *pointer;
			typedef _Tp &reference;
			typedef std::bidirectional_iterator_tag iterator_category;

		private:
			typedef _Node node_type;

			node_type *node;

		public:
			bst_iterator(): node(NULL) {}
			bst_iterator(node_type *node): node(node) {}
			bst_iterator(const bst_iterator &other): node(other.node) {}
			~bst_iterator() {}

			bst_iterator &operator=(const bst_iterator &rhs) {
				node = rhs.node;
				return (*this);
			}

			bool operator==(const bst_iterator &rhs) const {
				return node == rhs.node;
			}

			bool operator!=(const bst_iterator &rhs) const {
				return !(*this == rhs);
			}

			reference operator*() const {
				return *(node->data);
			}

			pointer operator->() const {
				return node->data;
			}

			bst_iterator &operator++() {
				if (node->right_node != NULL) {
					for (node = node->right_node; node->left_node != NULL && node->data != NULL; node = node->left_node) {}
				} else {
					while (true) {
						if (node->parent == NULL) {
							throw std::runtime_error("bst_iterator::operator++: no anchor node set (or properly set)");
						}

						if (node == node->parent->left_node) {
							node = node->parent;
							break;
						} else if (node == node->parent->right_node) {
							node = node->parent;
						} else {
							throw std::runtime_error("bst_iterator::operator++: could not determine current node side");
						}
					}
				}

				return (*this);
			}

			bst_iterator operator++(int) {
				bst_iterator value = *this;
				operator++();
				return value;
			}

			bst_iterator &operator--() {
				if (node->left_node != NULL) {
					// Yeah... We need to be able to decrement from end(), but... Not incrementing from begin() - 1 because it is not decrementable by definition
					// "The begin iterator is not decrementable and the behavior is undefined if --container.begin() is evaluated." -- from cppreference.com - LegacyBidirectionalIterator
					for (node = node->left_node; node->right_node != NULL && node->right_node->data != NULL; node = node->right_node) {}
				} else {
					while (true) {
						if (node->parent == NULL) {
							throw std::runtime_error("bst_iterator::operator--: no anchor node set (or properly set)");
						}

						if (node == node->parent->right_node) {
							node = node->parent;
							break;
						} else if (node == node->parent->left_node) {
							node = node->parent;
						} else {
							throw std::runtime_error("bst_iterator::operator--: could not determine current node side");
						}
					}
				}

				return (*this);
			}

			bst_iterator operator--(int) {
				bst_iterator value = *this;
				operator--();
				return value;
			}

			_Node *__get_node() {
				return node;
			}

			_Node *__get_node() const {
				return node;
			}
		};
	} // namespace __clsaad_impl
} // namespace ft
