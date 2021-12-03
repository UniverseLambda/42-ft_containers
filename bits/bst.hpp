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
			BSTNode *parent;

			_Key key;
			_Value value;

			BSTNodeColor nodeColor;
			BSTNode *leftNode;
			BSTNode *rightNode;

			BSTNode(BSTNode *parent, _Key key, _Value value):
				parent(parent),
				key(key),
				value(value),
				nodeColor(RED),
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

				if (key < aKey) {
					push_on_node(leftNode);
				} else {
					push_on_node(rightNode);
				}
			}

			_Value &find_value(const _Key &aKey) const {
				if (aKey == key) {
					return value;
				}

				return find_on_node((aKey < key) ? leftNode : rightNode);
			}

			void right_rotate() {
				BSTNode *pivot;

				if (!leftNode) {
					return;
				}

				pivot = leftNode;

				leftNode = pivot->rightNode;
				pivot->rightNode = this;

				if (parent) {
					parent->node_storage(this) = pivot;
				}
			}

			void left_rotate() {
				BSTNode *pivot;

				if (!rightNode) {
					return;
				}

				pivot = rightNode;

				rightNode = pivot->leftNode;
				pivot->leftNode = this;

				if (parent) {
					parent->node_storage(this) = pivot;
				}

			}

		private:
			inline BSTNode *sibling(BSTNode *node) {
				return (node == leftNode) ? rightNode : ((node == rightNode) ? leftNode : NULL);
			}

			inline BSTNode *&node_storage(BSTNode *node) {
				return (node == leftNode) ? leftNode : ((node == rightNode) ? rightNode : NULL);
			}

			inline void push_on_node(const _Key &aKey, const _Value &aValue, BSTNode *&node) {
				if (!node) {
					node = new BSTNode(this, aKey, aValue, BLACK);
				} else {
					node->push_value(aKey, aValue);
				}
			}

			_Value &find_on_node(const _Key &aKey, BSTNode *&node) {
				if (!node) {
					std::__throw_out_of_range("key node found");
				}

				return node->find_on_node(aKey);
			}
		};

	} // namespace __clsaad_impl
} // namespace ft
