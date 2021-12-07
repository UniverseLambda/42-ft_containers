#pragma once

#ifndef __FT_BITS_INC
# error "You included a bits header"
#endif

#include <functional>

namespace ft {
	namespace __clsaad_impl {
		enum BSTNodeColor {
			BLACK,
			RED
		};

		template<typename _Key, typename _Value, typename _Less = std::less<_Key> >
		struct BSTNode {
			BSTNode **root;
			BSTNode *parent;
			static BSTNode *const nullValue;

			_Key key;
			_Value value;

			BSTNodeColor nodeColor;
			BSTNode *leftNode;
			BSTNode *rightNode;

			BSTNode(BSTNode **root, BSTNode *parent, _Key key, _Value value, BSTNodeColor color = RED):
				root(root),
				parent(parent),
				key(key),
				value(value),
				nodeColor(color),
				leftNode(NULL),
				rightNode(NULL) {
				if (!parent) {
					nodeColor = BLACK;
				}
			}
		private:
			BSTNode(const BSTNode &) {}
		public:
			~BSTNode() {}

			void push_value(const _Key &aKey, const _Value &aValue) {
				if (key == aKey) {
					value = aValue;
				}

				if (aKey < key) {
					push_on_node(leftNode, aKey, aValue);
				} else {
					push_on_node(rightNode, aKey, aValue);
				}
			}

			_Value &find_value(const _Key &aKey) {
				if (aKey == key) {
					return value;
				}

				return find_on_node((aKey < key) ? leftNode : rightNode, aKey);
			}

			const _Value &find_value(const _Key &aKey) const {
				if (aKey == key) {
					return value;
				}

				return find_on_node((aKey < key) ? leftNode : rightNode, aKey);
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

		private:
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
					node = new BSTNode(root, this, aKey, aValue, RED);
					node->rebalance_tree();
				} else {
					node->push_value(aKey, aValue);
				}
			}

			_Value &find_on_node(BSTNode *&node, const _Key &aKey) {
				if (!node) {
					std::__throw_out_of_range("key node found");
				}

				return node->find_value(aKey);
			}

			const _Value &find_on_node(BSTNode *&node, const _Key &aKey) const {
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
		};
	} // namespace __clsaad_impl
} // namespace ft
