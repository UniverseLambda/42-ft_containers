#pragma once

#ifndef __FT_BITS_INC
# error "You included a bits header"
#endif

#include <functional>
#include <memory>
#include "../utility.hpp"

namespace ft {
	namespace __clsaad_impl {
		enum BSTNodeColor {
			BLACK,
			RED
		};

		template<typename _Tp>
		inline bool safe_callable(_Tp *self) {
			return self != NULL;
		}

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

		template<typename _Tree, typename _Data>
		struct BSTNode {
			static BSTNode *const nullValue;

			_Tree &tree;

			BSTNode **root;
			BSTNode *parent;
			
			_Data *data;

			BSTNodeColor nodeColor;

			BSTNode *leftNode;
			BSTNode *rightNode;

			BSTNode(_Tree &tree, BSTNode *parent, _Data *data, BSTNodeColor color):
				tree(tree),
				root(tree.get_root()),
				parent(parent),
				data(data),
				nodeColor(color),
				leftNode(NULL),
				rightNode(NULL) {
				if (!parent) {
					nodeColor = BLACK;
				}
			}

			BSTNode(_Tree &tree, const BSTNode &other):
				tree(tree),
				root(tree.get_root()),
				parent(NULL),
				data(bst_copy_data(tree.get_allocator(), other.data)),
				nodeColor(other.nodeColor),
				leftNode(NULL),
				rightNode(NULL) {
				
				if (other.leftNode != NULL) {
					leftNode = new BSTNode(tree, *(other.leftNode));
					leftNode->parent = this;
				}

				if (other.rightNode != NULL) {
					rightNode = new BSTNode(tree, *(other.rightNode));
					rightNode->parent = this;
				}

			}

		private:
			// `= delete' only exists since C++11. So we force it to just... Not compile
			BSTNode(const BSTNode &) {
				this->you_should_definitely_not_use_this_constructor_you_bad_boy();
			}

		public:
			~BSTNode() {
				if (rightNode && rightNode->data != NULL)
					delete rightNode;

				if (leftNode && leftNode->data != NULL)
					delete leftNode;

				bst_delete_data(tree.get_allocator(), data);
			}

			BSTNode &operator=(const BSTNode &) {
				return (*this);
			}

			void insertion_rebalance_tree() {
				BSTNode *grandparent;
				BSTNode *uncle;

				if (*root == this || parent == NULL) {
					nodeColor = BLACK;
					return;
				}

				if (parent->nodeColor == BLACK) {
					return;
				}

				grandparent = parent->parent;
				uncle = grandparent->sibling(parent);

				if (uncle != NULL && uncle->nodeColor == RED) {
					parent->nodeColor = BLACK;
					uncle->nodeColor = BLACK;
					grandparent->nodeColor = RED;
					grandparent->insertion_rebalance_tree();
				} else {
					if (grandparent->leftNode == parent && parent->leftNode == this) {
						rebalance_ll(grandparent, parent);
					} else if (grandparent->leftNode == parent && parent->rightNode == this) {
						rebalance_lr(grandparent, parent);
					} else if (grandparent->rightNode == parent && parent->rightNode == this) {
						rebalance_rr(grandparent, parent);
					} else if (grandparent->rightNode == parent && parent->leftNode == this) {
						rebalance_rl(grandparent, parent);
					} else {
						std::__throw_runtime_error("WHAT????");
					}
				}
			}

			// TODO: check iterator invalidation compliance with standard
			// void bst_deletion() {
			// 	std::size_t childCount = !!(leftNode) + !!(rightNode);

			// 	if (childCount == 2) {
			// 		BSTNode *swapped = rightNode->min_node();

			// 		swap_place(swapped);
			// 		swapped->bst_deletion();
			// 		return;
			// 	}

			// 	BSTNode *replacement = (leftNode) ? leftNode : rightNode;

			// 	if (parent != NULL) {
			// 		parent->node_storage(this) = replacement;
			// 	} else {
			// 		*root = replacement;
			// 	}

			// 	if (replacement != NULL) {
			// 		replacement->parent = parent;
			// 	}

			// 	replacement->remove_tree_fix(parent);

			// 	rightNode = NULL;
			// 	leftNode = NULL;
			// 	delete this;
			// }

			void right_rotate() {
				BSTNode *pivot;

				if (!leftNode) {
					return;
				}

				pivot = leftNode;

				leftNode = pivot->rightNode;

				if (leftNode)
					leftNode->parent = this;

				pivot->rightNode = this;

				pivot->parent = parent;

				if (parent) {
					parent->node_storage(this) = pivot;
				} else if (root) {
					*root = pivot;
				}

				parent = pivot;
			}

			void left_rotate() {
				BSTNode *pivot;

				if (!rightNode) {
					return;
				}

				pivot = rightNode;

				rightNode = pivot->leftNode;
				if (rightNode)
					rightNode->parent = this;

				pivot->leftNode = this;
				pivot->parent = parent;

				if (parent) {
					parent->node_storage(this) = pivot;
				} else if (root) {
					*root = pivot;
				}

				parent = pivot;
			}

			// TODO: transform this to iterative to avoid stack-overflow
			BSTNode *min_node() {
				if (leftNode == NULL || leftNode->data == NULL) {
					return this;
				}
				return leftNode->min_node();
			}

			inline BSTNode *sibling(BSTNode *node) {
				return (node == leftNode) ? rightNode : ((node == rightNode) ? leftNode : NULL);
			}

			inline BSTNode *&node_storage(BSTNode *node) {
				if (node == leftNode) {
					return leftNode;
				}

				if (node == rightNode) {
					return rightNode;
				}

				std::__throw_runtime_error("internal error");
			}

			// void propagate_double_black() {
			// 	if (parent == NULL) {
			// 		// THIS IS THE ROOT, SO DON'T DO ANYTHING ASIDE OF RECOLORING IT
			// 		nodeColor = BLACK;
			// 		return;
			// 	}

			// 	BSTNode *sibling = parent->sibling(this);

			// 	if (BSTNode::get_color(sibling) == RED) {
			// 		if (sibling == parent->leftNode) {
			// 			parent->left_rotate();
			// 			std::swap(parent->nodeColor, sibling->nodeColor);
						
			// 		} else {
			// 			parent->right_rotate();
			// 			std::swap(parent->nodeColor, sibling->nodeColor);
			// 		}

			// 		sibling = parent->sibling(this);
			// 	}

			// 	BSTNode *sibling_left = BSTNode::get_left_node(sibling);
			// 	BSTNode *sibling_right = BSTNode::get_right_node(sibling);

			// 	if (BSTNode::get_color(sibling) == BLACK) {
			// 		BSTNode *sibling_red = NULL;

			// 		if (BSTNode::get_color(sibling_right) == RED) {
			// 			sibling_red = sibling_right;
			// 		} else if (BSTNode::get_color(sibling_left) == RED) {
			// 			sibling_red = sibling_left;
			// 		}

			// 		if (sibling_red != NULL) {
			// 			if (sibling == parent->rightNode) {
			// 				if (sibling_red == sibling_right) {
			// 					parent->right_rotate();
			// 					sibling_right->nodeColor = BLACK;
			// 				} else {
			// 					sibling->left_rotate();
			// 					std::swap(sibling->nodeColor, sibling_red->nodeColor);

			// 					parent->right_rotate();
			// 					sibling->nodeColor = BLACK;
			// 				}
			// 			} else {
			// 				if (sibling_red == sibling_left) {
			// 					parent->left_rotate();
			// 					sibling_left->nodeColor = BLACK;
			// 				} else {
			// 					sibling->right_rotate();
			// 					std::swap(sibling->nodeColor, sibling_red->nodeColor);

			// 					parent->left_rotate();
			// 					sibling->nodeColor = BLACK;
			// 				}
			// 			}

			// 		} else {
			// 			BSTNode::set_color(sibling, RED);
						
			// 			if (parent->nodeColor == BLACK) {
			// 				parent->propagate_double_black();
			// 			} else {
			// 				parent->nodeColor = BLACK;
			// 			}
			// 		}
			// 	}
			// }

		private:
			void rebalance_ll(BSTNode *grandparent, BSTNode *parent) {
				BSTNodeColor tmp;

				grandparent->right_rotate();

				tmp = parent->nodeColor;
				parent->nodeColor = grandparent->nodeColor;
				grandparent->nodeColor = tmp;
			}

			void rebalance_lr(BSTNode *grandparent, BSTNode *parent) {
				BSTNodeColor tmp;

				parent->left_rotate();
				grandparent->right_rotate();

				tmp = nodeColor;
				nodeColor = grandparent->nodeColor;
				grandparent->nodeColor = tmp;
			}

			void rebalance_rr(BSTNode *grandparent, BSTNode *parent) {
				BSTNodeColor tmp;

				grandparent->left_rotate();

				tmp = parent->nodeColor;
				parent->nodeColor = grandparent->nodeColor;
				grandparent->nodeColor = tmp;
			}

			void rebalance_rl(BSTNode *grandparent, BSTNode *parent) {
				BSTNodeColor tmp;

				parent->right_rotate();
				grandparent->left_rotate();

				tmp = nodeColor;
				nodeColor = grandparent->nodeColor;
				grandparent->nodeColor = tmp;
			}

			// void swap_place(BSTNode *node) {
			// 	std::swap(data, node->data);

			// 	// Mostly used with pointers. This whole shit is useless. FML

			// 	// if (parent) {
			// 	// 	parent->node_storage(this) = node;
			// 	// } else {
			// 	// 	*root = node;
			// 	// }

			// 	// if (node->parent) {
			// 	// 	node->parent->node_storage(node) = this;
			// 	// } else {
			// 	// 	*root = this;
			// 	// }

			// 	// std::swap(rightNode, node->rightNode);
			// 	// std::swap(leftNode, node->leftNode);
			// 	// std::swap(parent, node->parent);
			// 	// std::swap(nodeColor, node->nodeColor);

			// 	// if (parent == this) {
			// 	// 	parent = node;
			// 	// 	((node->rightNode == node) ? node->rightNode : node->leftNode) = this;
			// 	// } else if (node->parent == node) {
			// 	// 	node->parent = this;
			// 	// 	((rightNode == this) ? rightNode : leftNode) = node;
			// 	// }

			// 	// if (rightNode) {
			// 	// 	rightNode->parent = this;
			// 	// }

			// 	// if (leftNode) {
			// 	// 	leftNode->parent = this;
			// 	// }

			// 	// if (node->rightNode) {
			// 	// 	node->rightNode->parent = node;
			// 	// }

			// 	// if (node->leftNode) {
			// 	// 	node->leftNode->parent = node;
			// 	// }
			// }

			// !! We starting to get into the shady stuff !!

			// void remove_tree_fix(BSTNode *parent) {

			// 	// TODO: Check behavior when replacement is now root
			// 	if (parent == NULL) {
			// 		return;
			// 	}

			// 	BSTNodeColor color = safe_get_color();
			// 	if (color == RED || parent->nodeColor == RED) {
			// 		safe_set_color(BLACK);
			// 		return;
			// 	}

			// 	if (parent == NULL) {

			// 	}
			// }

			// !!!!!! WARNING Black magic wizardry !!!!!!

			// BSTNodeColor safe_get_color() const {
			// 	return (safe_callable(this)) ? nodeColor : BLACK;
			// }

			// void safe_set_color(BSTNodeColor color) {
			// 	if (safe_callable(this)) {
			// 		nodeColor = color;
			// 	}
			// }

			// BSTNode *safe_get_left_node() const {
			// 	return (safe_callable(this)) ? leftNode : NULL;
			// }

			// BSTNode *safe_get_right_node() const {
			// 	return (safe_callable(this)) ? rightNode : NULL;
			// }

		public:
			static void set_color(BSTNode *node, BSTNodeColor color) {
				if (node != NULL) {
					node->nodeColor = color;
				}
			}

			static BSTNodeColor get_color(BSTNode *node) {
				return (node != NULL) ? node->nodeColor : BLACK;
			}

			static BSTNode *get_left_node(BSTNode *node) {
				return (node != NULL) ? node->leftNode : NULL;
			}

			static BSTNode *get_right_node(BSTNode *node) {
				return (node != NULL) ? node->rightNode : NULL;
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
			friend bst_wrapper_anchor_guard<bst_wrapper>;
		public:
			typedef BSTNode<bst_wrapper, _Data> node_type;
			typedef bst_wrapper_anchor_guard<bst_wrapper> anchor_guard_type;

		private:
			_Less less;
			_Allocator alloc;
			_KeyExtractor key_extractor;
			_KeyLess key_less;

			node_type *root;
			node_type anchor;

		public:
			bst_wrapper(const _Less &less = _Less(), const _Allocator &alloc = _Allocator(), const _KeyExtractor &key_extractor = _KeyExtractor(), const _KeyLess &key_less = _KeyLess()):
				less(less),
				alloc(alloc),
				key_extractor(key_extractor),
				key_less(key_less),
				root(NULL),
				anchor((*this), NULL, NULL, BLACK)
				{}

			bst_wrapper(const bst_wrapper &other):
				less(other.less),
				alloc(other.alloc),
				key_extractor(other.key_extractor),
				key_less(other.key_less),
				root(NULL),
				anchor((*this), NULL, NULL, BLACK) {

				if (other.root != NULL) {
					root = new node_type(*this, *other.root);
				}

				set_anchor();
			}
			
			~bst_wrapper() {
				anchor.rightNode = NULL;
				anchor.leftNode = NULL;
				if (root != NULL) {
					delete root;
				}
			}

			bst_wrapper &operator=(const bst_wrapper &rhs) {
				if (root != NULL)
					delete root;

				if (rhs.root == NULL) {
					root = NULL;
				} else {
					root = new node_type(*rhs.root, *this);
				}

				return (*this);
			}

		private:
			void set_anchor() {
				if (root == NULL) {
					anchor.leftNode = NULL;
					anchor.rightNode = NULL;
					return;
				}

				node_type *left_node = root;
				node_type *right_node = root;

				while (left_node->leftNode != NULL || right_node->rightNode != NULL) {
					if (left_node->leftNode != NULL) {
						left_node = left_node->leftNode;
					}

					if (right_node->rightNode != NULL) {
						right_node = right_node->rightNode;
					}
				}

				anchor.leftNode = left_node;
				anchor.rightNode = right_node;
			}

			void remove_anchor() {
				if (anchor.leftNode)
					anchor.leftNode->leftNode = NULL;
				if (anchor.rightNode)
					anchor.rightNode->rightNode = NULL;
				anchor.leftNode = NULL;
				anchor.rightNode = NULL;
			}

		public:
			ft::pair<node_type *, bool> insert(const _Data &data) {
				if (root == NULL) {
					root = new node_type((*this), NULL, bst_allocate(alloc, data), BLACK);
					set_anchor();
					return ft::make_pair(root, true);
				}

				node_type *parent;
				bool is_less;

				node_type *node = retrieve_node(data, parent, is_less);

				if (node != NULL) {
					return ft::make_pair(node, false);
				}

				anchor_guard_type guard(*this);
				node = new node_type(*this, parent, bst_allocate(alloc, data), RED);

				if (is_less) {
					parent->leftNode = node;
				} else {
					parent->rightNode = node;
				}

				node->insertion_rebalance_tree();
				return ft::make_pair(node, true);
			}

			node_type &find_node(const _Key &key) {
				node_type *result = retrieve_node_key(key);

				if (result == NULL)
					std::__throw_out_of_range("key not found");
				
				return *result;
			}

			const node_type &find_node(const _Key &key) const {
				node_type *result = retrieve_node_key(key);

				if (result != NULL)
					std::__throw_out_of_range("key not found");
				
				return *result;
			}

			bool erase(const _Key &key) {
				node_type *node = retrieve_node_key(key);

				if (!node) {
					return false;
				}

				erase(node);
				return true;
			}

			void erase(node_type *node) {
				anchor_guard_type guard(*this);

				int child_count = !!(node->leftNode) + !!(node->rightNode);

				if (child_count == 2) {
					swap_places(node, node->rightNode->min_node());
					child_count = !!(node->leftNode) + !!(node->rightNode);
				}

				node_type *replacement = (node->leftNode != NULL) ? node->leftNode : node->rightNode;

				if (replacement != NULL) {
					replacement->parent = node->parent;
				}

				if (root == node) {
					root = replacement;
					node_type::set_color(replacement, BLACK);
				} else {
					node->parent->node_storage(node) = replacement;

					if (node->nodeColor == RED || node_type::get_color(replacement) == RED) {
						node_type::set_color(replacement, BLACK);
					} else {
						propagate_double_black(replacement, node);
					}
				}

				// Setting them to NULL to prevent them from being deleted
				node->leftNode = NULL;
				node->rightNode = NULL;
				delete node;
			}

			_Allocator &get_allocator() {
				return alloc;
			}

			const _Allocator &get_allocator() const {
				return alloc;
			}

			node_type **get_root() {
				return &root;
			}

			const node_type **get_root() const {
				return &root;
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
						next_node = current_node->leftNode;
						current_is_less = true;
					} else {
						next_node = current_node->rightNode;
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
						next_node = current_node->leftNode;
						current_is_less = true;
					} else {
						next_node = current_node->rightNode;
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
				std::swap(n0->leftNode, n1->leftNode);
				std::swap(n0->rightNode, n1->rightNode);
				std::swap(n0->parent, n1->parent);
				std::swap(n0->nodeColor, n1->nodeColor);

				if (n0->parent == NULL) {
					root = n0;
				} else {
					if (n0->parent == n0) {
						n0->parent = n1;
						((n1->leftNode == n1) ? n1->leftNode : n1->rightNode) = n0;
					} else {
						n0->parent->node_storage(n1) = n0;
					}
				}

				if (n1->parent == NULL) {
					root = n1;
				} else {
					if (n1->parent == n1) {
						n1->parent = n0;
						((n0->leftNode == n0) ? n0->leftNode : n0->rightNode) = n1;
					} else {
						n1->parent->node_storage(n0) = n1;
					}
				}

				if (n0->leftNode != NULL) n0->leftNode->parent = n0;
				if (n0->rightNode != NULL) n0->rightNode->parent = n0;
				if (n1->leftNode != NULL) n1->leftNode->parent = n1;
				if (n1->rightNode != NULL) n1->rightNode->parent = n1;
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
					if (sibling == node_this->parent->leftNode) {
						node_this->parent->left_rotate();
						std::swap(node_this->parent->nodeColor, sibling->nodeColor);
						
					} else {
						node_this->parent->right_rotate();
						std::swap(node_this->parent->nodeColor, sibling->nodeColor);
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
						if (sibling == node_this->parent->rightNode) {
							if (sibling_red == sibling_right) {
								node_this->parent->right_rotate();
								sibling_right->nodeColor = BLACK;
							} else {
								sibling->left_rotate();
								std::swap(sibling->nodeColor, sibling_red->nodeColor);

								node_this->parent->right_rotate();
								sibling->nodeColor = BLACK;
							}
						} else {
							if (sibling_red == sibling_left) {
								node_this->parent->left_rotate();
								sibling_left->nodeColor = BLACK;
							} else {
								sibling->right_rotate();
								std::swap(sibling->nodeColor, sibling_red->nodeColor);

								node_this->parent->left_rotate();
								sibling->nodeColor = BLACK;
							}
						}

					} else {
						node_type::set_color(sibling, RED);
						
						if (node_this->parent->nodeColor == BLACK) {
							propagate_double_black(node_this->parent);
						} else {
							node_this->parent->nodeColor = BLACK;
						}
					}
				}
			}
		};

	} // namespace __clsaad_impl
} // namespace ft
