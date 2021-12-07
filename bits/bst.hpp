#pragma once

#ifndef __FT_BITS_INC
# error "You included a bits header"
#endif

namespace ft {
	namespace __clsaad_impl {
		enum BSTNodeColor {
			BLACK,
			RED
		};

		template<typename _Key, typename _Value>
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
					node = new BSTNode(root, this, aKey, aValue, BLACK);
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
		};
	} // namespace __clsaad_impl
} // namespace ft
