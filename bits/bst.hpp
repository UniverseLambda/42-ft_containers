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

		template<typename _Key, typename _Value, typename _Less = std::less<_Key>, typename _Allocator = std::allocator<pair<const _Key, _Value> > >
		struct BSTNode {
			BSTNode **root;
			BSTNode *parent;
			_Less less;
			static BSTNode *const nullValue;

			typedef pair<const _Key, _Value> pairType;

			_Allocator alloc;
			pairType *data;

			BSTNodeColor nodeColor;
			BSTNode *leftNode;
			BSTNode *rightNode;

			BSTNode(BSTNode **root, BSTNode *parent, _Less less, const _Key &key, const _Value &value, _Allocator alloc, BSTNodeColor color = RED):
				root(root),
				parent(parent),
				less(less),
				alloc(alloc),
				data(alloc.allocate(1)),
				nodeColor(color),
				leftNode(NULL),
				rightNode(NULL) {
				if (!parent) {
					nodeColor = BLACK;
				}

				alloc.construct(data, key, value);
			}

		private:
			BSTNode(const BSTNode &) {}
		public:
			~BSTNode() {
				if (rightNode)
					delete rightNode;

				if (leftNode)
					delete leftNode;

				alloc.destroy(data);
				alloc.deallocate(data, 1);
			}

		private:
			BSTNode &operator=(const BSTNode &) {
				return (*this);
			}

		public:
			void push_value(const _Key &aKey, const _Value &aValue) {
				if (key_equivalent(aKey)) {
					data->second = aValue;
				}

				if (less(aKey, data->first)) {
					push_on_node(leftNode, aKey, aValue);
				} else {
					push_on_node(rightNode, aKey, aValue);
				}
			}

			_Value &find_value(const _Key &aKey) {
				if (key_equivalent(aKey)) {
					return data->second;
				}

				return find_on_node((less(aKey, data->first)) ? leftNode : rightNode, aKey);
			}

			bool remove_value(const _Key &aKey) {
				if (!key_equivalent(aKey)) {
					return remove_on_node((less(aKey, data->first)) ? leftNode : rightNode, aKey);
				}

				bst_deletion();
				return true;
			}

			const _Value &find_value(const _Key &aKey) const {
				if (key_equivalent(aKey)) {
					return data->second;
				}

				return find_on_node((_Less(aKey, data->first)) ? leftNode : rightNode, aKey);
			}

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

			BSTNode *min_node() {
				if (leftNode == NULL) {
					return this;
				}
				return leftNode->min_node();
			}

		private:
			inline bool key_equivalent(const _Key &k) {
				return !less(data->first, k) && !less(k, data->first);
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

			inline void push_on_node(BSTNode *&node, const _Key &aKey, const _Value &aValue) {
				if (!node) {
					node = new BSTNode(root, this, less, aKey, aValue, alloc, RED);
					node->rebalance_tree();
				} else {
					node->push_value(aKey, aValue);
				}
			}

			inline bool remove_on_node(BSTNode *node, const _Key &aKey) {
				if (!node) {
					return false;
				}
				return node->remove_value(aKey);
			}

			_Value &find_on_node(BSTNode *node, const _Key &aKey) {
				if (!node) {
					std::__throw_out_of_range("key node found");
				}

				return node->find_value(aKey);
			}

			const _Value &find_on_node(BSTNode *node, const _Key &aKey) const {
				if (!node) {
					std::__throw_out_of_range("key node found");
				}

				return node->find_value(aKey);
			}

			void rebalance_tree() {
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
					grandparent->rebalance_tree();
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

			void swap_place(BSTNode *node) {
				std::swap(data, node->data);

				// Mostly used with pointers. This whole shit is useless. FML

				// if (parent) {
				// 	parent->node_storage(this) = node;
				// } else {
				// 	*root = node;
				// }

				// if (node->parent) {
				// 	node->parent->node_storage(node) = this;
				// } else {
				// 	*root = this;
				// }

				// std::swap(rightNode, node->rightNode);
				// std::swap(leftNode, node->leftNode);
				// std::swap(parent, node->parent);
				// std::swap(nodeColor, node->nodeColor);

				// if (parent == this) {
				// 	parent = node;
				// 	((node->rightNode == node) ? node->rightNode : node->leftNode) = this;
				// } else if (node->parent == node) {
				// 	node->parent = this;
				// 	((rightNode == this) ? rightNode : leftNode) = node;
				// }

				// if (rightNode) {
				// 	rightNode->parent = this;
				// }

				// if (leftNode) {
				// 	leftNode->parent = this;
				// }

				// if (node->rightNode) {
				// 	node->rightNode->parent = node;
				// }

				// if (node->leftNode) {
				// 	node->leftNode->parent = node;
				// }
			}

			void bst_deletion() {
				std::size_t childCount = !!(leftNode) + !!(rightNode);

				if (childCount == 2) {
					BSTNode *swapped = rightNode->min_node();

					swap_place(swapped);
					swapped->bst_deletion();
					return;
				}

				BSTNode *replacement = (leftNode) ? leftNode : rightNode;

				if (parent != NULL) {
					parent->node_storage(this) = replacement;
				} else {
					*root = replacement;
				}

				if (replacement != NULL) {
					replacement->parent = parent;
				}

				replacement->remove_tree_fix(parent);

				rightNode = NULL;
				leftNode = NULL;
				delete this;
			}

			// !! We starting to get into the shady stuff !!

			void remove_tree_fix(BSTNode *parent) {

				// TODO: Check behavior when replacement is now root
				if (parent == NULL) {
					return;
				}

				BSTNodeColor color = safe_get_color();
				if (color == RED || parent->nodeColor == RED) {
					safe_set_color(BLACK);
					return;
				}

				BSTNode *sibling = parent->sibling(this);

				if (sibling->safe_get_color() == RED) {
					if (sibling == parent->rightNode) {
						// Right case
						parent->left_rotate();

					} else {
						// Left case
						parent->right_rotate();
					}

					sibling->nodeColor = BLACK;
					parent->nodeColor = RED;

					sibling = parent->sibling(this);
				}

				if (sibling->safe_get_left_node()->safe_get_color() == BLACK &&
					sibling->safe_get_right_node()->safe_get_color() == BLACK) {
					parent->propagate_double_black(parent->parent);
				} else {
					// BSTNode *left = sibling->leftNode;
					BSTNode *right = sibling->rightNode;

					if (sibling == parent->rightNode) {
						// Right Left case
						if (right->safe_get_color() != RED) {
							sibling->right_rotate();
							std::swap(rightNode, sibling);
						}

						// Right Right case
						parent->left_rotate();
						rightNode->nodeColor = BLACK;
					} else {
						// Left Left case
						if (right->safe_get_color() != RED) {
							sibling->left_rotate();
							std::swap(rightNode, sibling);
						}

						// Left Right case
						parent->right_rotate();
						rightNode->nodeColor = BLACK;
					}
				}
			}

			void propagate_double_black(BSTNode *parent) {
				if (parent == NULL) {
					// THIS IS THE ROOT, SO DON'T DO ANYTHING
					return;
				}

				parent->sibling(this)->safe_set_color(RED);

				if (parent->nodeColor == BLACK) {
					parent->propagate_double_black(parent->parent);
				} else {
					parent->nodeColor = RED;
				}
			}

			// !!!!!! WARNING Black magic wizardry !!!!!!

			BSTNodeColor safe_get_color() const {
				return (safe_callable(this)) ? nodeColor : BLACK;
			}

			void safe_set_color(BSTNodeColor color) {
				if (safe_callable(this)) {
					nodeColor = color;
				}
			}

			BSTNode *safe_get_left_node() const {
				return (safe_callable(this)) ? leftNode : NULL;
			}

			BSTNode *safe_get_right_node() const {
				return (safe_callable(this)) ? rightNode : NULL;
			}
		};
	} // namespace __clsaad_impl
} // namespace ft
