#include "iterator.hpp"
#include "type_traits.hpp"
#include "algorithm.hpp"
#include "utility.hpp"

#include "Vector.hpp"

#include "Stack.hpp"

#include "Map.hpp"

#include <ostream>
#include <iostream>
#include <iomanip>

#ifndef DSP_CHECK_VAL
# define DSP_CHECK_VAL true
#endif

#define VECTOR_TEST(X) X; \
						checkVector(validity, testIdx++)

struct CompareExpected {
	enum _CompareExpectedV {
		EQUALS,
		GREATER,
		LOWER
	};
};

struct ValiditySanitizer {
	enum Origin {
		CTOR,
		CPY_CTOR,
		CPY_OP,
		DTOR,
	};

	bool valid;
	Origin origin;
	std::size_t idx;

	ValiditySanitizer(size_t idx): valid(true), origin(CTOR), idx(idx) {}
	ValiditySanitizer(const ValiditySanitizer &other): valid(other.valid), origin(CPY_CTOR), idx(other.idx) {}

	~ValiditySanitizer() {
		valid = false;
		origin = DTOR;
	}

	ValiditySanitizer &operator=(const ValiditySanitizer &lhs) {
		valid = lhs.valid;
		origin = CPY_OP;
		idx = lhs.idx;

		return (*this);
	}

	void check() const {
		if (!valid) {
			std::__throw_runtime_error((std::string("not valid object (") + originToString(origin) + ")").data());
		}
	}

	bool operator==(const ValiditySanitizer &rhs) const {
		return (idx == rhs.idx);
	}

	bool operator<(const ValiditySanitizer &rhs) const {
		return (idx < rhs.idx);
	}

	static std::string originToString(Origin origin) {
		switch (origin) {
		case CTOR:		return "CTOR";
		case CPY_CTOR:	return "CPY_CTOR";
		case CPY_OP:	return "CPY_OP";
		case DTOR:		return "DTOR";
		}
		return "UNKNOWN";
	}
};

std::ostream &operator<<(std::ostream &os, ValiditySanitizer::Origin origin) {
	return (os << ValiditySanitizer::originToString(origin));
}

void checkVector(const ft::vector<ValiditySanitizer> &validity, std::size_t testIdx) {
	typedef ft::vector<ValiditySanitizer>::const_iterator iter;

	std::size_t idx = 0;

	std::cout << "== CHECK " << testIdx << " (current size: " << validity.size() << " out of " << validity.capacity() << ") ==" << std::endl;
	for (iter it = validity.begin(); it != validity.end(); ++idx, ++it) {
		if (DSP_CHECK_VAL) {
			if (idx < 31) {
				std::cout << "Checking [" << idx << "] " << it->idx << std::endl;
			} else if (idx == 31) {
				std::cout << "(...)" << std::endl;
			} else if ((it + 1 == validity.end()) || (it + 2 == validity.end())) {
				std::cout << "Checking [" << idx << "] " << it->idx << std::endl;
			}
		}
		it->check();
	}
}

void insertFromOtherVector(ft::vector<ValiditySanitizer> &validity, std::size_t &testIdx) {
	ft::vector<ValiditySanitizer> validity1;


	for (std::size_t i = 0; i < 10; ++i) {
		validity1.push_back(ValiditySanitizer(i + 1000));
	}

	VECTOR_TEST(validity.insert(validity.begin() + 9, validity1.begin(), validity1.end()));
}

void assignFromOtherVector(ft::vector<ValiditySanitizer> &validity, std::size_t &testIdx) {
	ft::vector<ValiditySanitizer> validity1;


	for (std::size_t i = 0; i < 20; ++i) {
		validity1.push_back(ValiditySanitizer(i + 6000));
	}

	VECTOR_TEST(validity.assign(validity1.begin(), validity1.end()));
}

void swapFromOtherVector(ft::vector<ValiditySanitizer> &validity, std::size_t &testIdx) {
	ft::vector<ValiditySanitizer> validity1;

	for (std::size_t i = 0; i < 20; ++i) {
		validity1.push_back(ValiditySanitizer(i + 10000));
	}

	VECTOR_TEST(validity.swap(validity1));

	for (std::size_t i = 0; i < 20; ++i) {
		validity1.push_back(ValiditySanitizer(i + 10000 + 20));
	}

	VECTOR_TEST(ft::swap(validity, validity1));
	VECTOR_TEST(ft::swap(validity, validity1));
}

template<typename _Tp>
void execCompareVector(const ft::vector<_Tp> &v0, const ft::vector<_Tp> &v1, CompareExpected::_CompareExpectedV result) {
	if ((v0 == v1) == (result == CompareExpected::EQUALS)) {
		std::cout << "\x1B[92m" << "== test passed :)" << "\x1B[0m" << std::endl;
	} else {
		std::cout << "\x1B[91m" << "== test not passed :(" << "\x1B[0m" << std::endl;
	}

	if ((v0 != v1) == (result != CompareExpected::EQUALS)) {
		std::cout << "\x1B[92m" << "!= test passed :)" << "\x1B[0m" << std::endl;
	} else {
		std::cout << "\x1B[91m" << "!= test not passed :(" << "\x1B[0m" << std::endl;
	}

	if ((v0 <= v1) == (result != CompareExpected::GREATER)) {
		std::cout << "\x1B[92m" << "<= test passed :)" << "\x1B[0m" << std::endl;
	} else {
		std::cout << "\x1B[91m" << "<= test not passed :(" << "\x1B[0m" << std::endl;
	}

	if ((v0 < v1) == (result == CompareExpected::LOWER)) {
		std::cout << "\x1B[92m" << "< test passed :)" << "\x1B[0m" << std::endl;
	} else {
		std::cout << "\x1B[91m" << "< test not passed :(" << "\x1B[0m" << std::endl;
	}

	if ((v0 >= v1) == (result != CompareExpected::LOWER)) {
		std::cout << "\x1B[92m" << ">= test passed :)" << "\x1B[0m" << std::endl;
	} else {
		std::cout << "\x1B[91m" << ">= test not passed :(" << "\x1B[0m" << std::endl;
	}

	if ((v0 > v1) == (result == CompareExpected::GREATER)) {
		std::cout << "\x1B[92m" << "> test passed :)" << "\x1B[0m" << std::endl;
	} else {
		std::cout << "\x1B[91m" << "> test not passed :(" << "\x1B[0m" << std::endl;
	}
}

void compareVector(ft::vector<ValiditySanitizer> &validity) {
	ft::vector<ValiditySanitizer> cmp(validity);

	std::cout << "*** Compare (equals) ***" << std::endl;
	execCompareVector(validity, cmp, CompareExpected::EQUALS);

	std::cout << "*** Compare (greater) ***" << std::endl;
	validity.back().idx++;
	execCompareVector(validity, cmp, CompareExpected::GREATER);

	std::cout << "*** Compare (lower) ***" << std::endl;
	validity.back().idx -= 2;
	execCompareVector(validity, cmp, CompareExpected::LOWER);

	std::cout << "*** Compare (lower size) ***" << std::endl;
	validity.pop_back();
	execCompareVector(validity, cmp, CompareExpected::LOWER);

	std::cout << "*** Compare (greater size) ***" << std::endl;
	validity.push_back(cmp.back());
	validity.push_back(validity.back());
	validity.back().idx--;
	execCompareVector(validity, cmp, CompareExpected::GREATER);

	std::cout << "*** Compare (both empty) ***" << std::endl;
	validity.clear();
	cmp.clear();
	execCompareVector(validity, cmp, CompareExpected::EQUALS);
}

void assignmentOperator(ft::vector<ValiditySanitizer> &validity, std::size_t &testIdx) {
	ft::vector<ValiditySanitizer> v1;

	// Java-style
	for (int i = 0; i < 16; ++i) {
		v1.push_back(ValiditySanitizer(i + 1000));
	}

	VECTOR_TEST(validity = v1);
}

void testRemainingCtor(ft::vector<ValiditySanitizer> &validity) {
	ft::vector<ValiditySanitizer> v0(validity.begin(), validity.end());
	ft::vector<ValiditySanitizer> v1(10, ValiditySanitizer(69420));

	std::cout << "**** iterator CTOR TEST **** (vvvvvv)" << std::endl;
	checkVector(v0, 0);

	std::cout << "**** count-val CTOR TEST **** (vvvvvv)" << std::endl;
	checkVector(v1, 0);
}

void vectorTest() {
	typedef ft::vector<ValiditySanitizer>::reverse_iterator riter;
	ft::vector<ValiditySanitizer> validity;
	std::size_t testIdx = 0;

	for (std::size_t i = 0; i < 5; ++i) {
		validity.push_back(ValiditySanitizer(i));
	}

	VECTOR_TEST();

	VECTOR_TEST(validity.insert(validity.begin() + 5, ValiditySanitizer(666)));

	VECTOR_TEST(validity.insert(validity.begin() + 5, 6, ValiditySanitizer(667)));

	VECTOR_TEST(validity.insert(validity.begin() + 2, 42, ValiditySanitizer(69420)));

	VECTOR_TEST(validity.insert(validity.begin() + 2, 0, ValiditySanitizer(99999)));

	VECTOR_TEST(insertFromOtherVector(validity, testIdx));

	VECTOR_TEST(validity.erase(validity.begin() + 5, validity.begin() + 10));

	VECTOR_TEST(validity.erase(validity.begin(), validity.begin()));

	VECTOR_TEST(validity.erase(validity.begin() + 5, validity.begin() + 5));

	VECTOR_TEST(validity.erase(validity.begin(), validity.end()));

	VECTOR_TEST(validity.erase(validity.begin(), validity.end()));

	for (std::size_t i = 0; i < 5; ++i) {
		validity.push_back(ValiditySanitizer(i + 2000));
	}

	VECTOR_TEST();
	VECTOR_TEST(validity.clear());
	VECTOR_TEST(validity.clear());

	VECTOR_TEST(for (std::size_t i = 0; i < 20; ++i) {
		validity.push_back(ValiditySanitizer(i + 3000));
	});

	VECTOR_TEST(while (!validity.empty()) {
		VECTOR_TEST(validity.pop_back());
	});

	VECTOR_TEST(for (std::size_t i = 0; i < 20; ++i) {
		validity.push_back(ValiditySanitizer(i + 4000));
	});

	std::cout << "== REV 0 ==" << std::endl;
	VECTOR_TEST(for (riter it = validity.rbegin(); it != validity.rend(); ++it) {
		std::cout << "Rev'd " << (*it).idx << std::endl;
	});

	std::cout << "== REV 1 ==" << std::endl;
	VECTOR_TEST(for (riter it = validity.rbegin(); it != validity.rend(); ++it) {
		std::cout << "Rev'd " << it->idx << std::endl;
	});

	VECTOR_TEST(validity.assign(15, ValiditySanitizer(5000)));
	VECTOR_TEST(assignFromOtherVector(validity, testIdx));
	VECTOR_TEST(validity.clear());
	assignFromOtherVector(validity, testIdx);

	std::cout << "== at TEST ==" << std::endl;
	VECTOR_TEST(for (std::size_t i = 0; i < validity.size(); ++i) {
		std::cout << "Elem " << i << " " << validity.at(i).idx << std::endl;
	});

	std::cout << "== [] TEST ==" << std::endl;
	VECTOR_TEST(for (std::size_t i = 0; i < validity.size(); ++i) {
		std::cout << "Elem " << i << " " << validity[i].idx << std::endl;
	});

	std::cout << "Front " << validity.front().idx << std::endl;
	std::cout << "Back " << validity.back().idx << std::endl;

	std::cout << "== data TEST ==" << std::endl;
	VECTOR_TEST(for (std::size_t i = 0; i < validity.size(); ++i) {
		std::cout << "Elem " << i << " " << validity.data()[i].idx << std::endl;
	});

	// Well, already tested vectors in and out (:
	// Empty, tested
	// Size...
	std::cout << "Max size  " << validity.max_size() << std::endl;

	// Reserve is used internally, but let's try with shaddy values
	VECTOR_TEST();
	VECTOR_TEST(validity.reserve(70));
	VECTOR_TEST(validity.reserve(0));
	VECTOR_TEST(validity.reserve(2));
	VECTOR_TEST(validity.reserve(3));
	VECTOR_TEST(validity.reserve(129));
	VECTOR_TEST(try {
		validity.reserve(validity.max_size() + 1);
		std::cout << "\x1B[91m" << "/!\\ No exception caught (shit)" << "\x1B[0m" << std::endl;
	} catch (std::exception &e) {
		std::cout << "\x1B[92m" << "/!\\ Caught an exception: " << e.what() << " (that's good)" << "\x1B[0m" << std::endl;
	});

	VECTOR_TEST(std::cout << "Shitty vector test: " << (2 + validity.begin())->idx << std::endl);


	// Capacity, already tested......
	// Clear, already tested..................
	// Insert, already tested............................
	// Erase, already tested.........................................
	// Push_back, already tested.............................................
	// Pop_back, already tested.................................................

	VECTOR_TEST(validity.resize(15, ValiditySanitizer(7000)));
	VECTOR_TEST(validity.resize(10, ValiditySanitizer(7000)));
	VECTOR_TEST(validity.resize(16, ValiditySanitizer(7000)));
	VECTOR_TEST(validity.resize(257, ValiditySanitizer(8000)));
	VECTOR_TEST(swapFromOtherVector(validity, testIdx));

	VECTOR_TEST(compareVector(validity));

	for (std::size_t i = 0; i < 20; ++i) {
		validity.push_back(ValiditySanitizer(i));
	}

	VECTOR_TEST(assignmentOperator(validity, testIdx));
	VECTOR_TEST(testRemainingCtor(validity));
}

void print_tree_branch(ft::__clsaad_impl::BSTNode<int, int> *node, int depth) {

	for (int i = 0; i < depth; ++i) {
		std::cout << "          ";
	}

	std::cout << std::setw(4);

	if (!node) {
		std::cout << "       _" << std::endl;
		return;
	}

	std::cout << node->value << " (" << (node->nodeColor == ft::__clsaad_impl::RED ? "R" : "B") << ")" << " ------- +" << std::endl;

	print_tree_branch(node->rightNode, depth + 1);
	print_tree_branch(node->leftNode, depth + 1);

}

template<typename _Key, typename _Value>
void sanitize_tree(ft::__clsaad_impl::BSTNode<_Key, _Value> *root) {
	if (!root) {
		return;
	}

	if (root->leftNode) {
		if (root->leftNode->parent != root) {
			std::cerr << "Error at node " << root->leftNode->value << std::endl;
			std::__throw_runtime_error("TREE_SANITIZER: Wrong parent");
		}

		if (root->leftNode->nodeColor == ft::__clsaad_impl::RED && root->leftNode->nodeColor == root->nodeColor) {
			std::cerr << "Error at node " << root->leftNode->value << std::endl;
			std::__throw_runtime_error("TREE_SANITIZER: Wrong color");
		}
	}

	if (root->rightNode) {
		if (root->rightNode->parent != root) {
			std::cerr << "Error at node " << root->rightNode->value << std::endl;
			std::__throw_runtime_error("TREE_SANITIZER: Wrong parent");
		}

		if (root->rightNode->nodeColor == ft::__clsaad_impl::RED && root->rightNode->nodeColor == root->nodeColor) {
			std::cerr << "Error at node " << root->rightNode->value << std::endl;
			std::__throw_runtime_error("TREE_SANITIZER: Wrong color");
		}
	}

	sanitize_tree(root->leftNode);
	sanitize_tree(root->rightNode);
}



void test_node() {
	ft::__clsaad_impl::BSTNode<int, int> *root;
	ft::__clsaad_impl::BSTNode<int, int> node(&root, NULL, std::less<int>(), 50, 50);
	root = &node;
	int k = 50;

	std::cout << "===== After inserting 30 =====" << std::endl;
	root->push_value(30, 30);
	print_tree_branch(root, 0);
	sanitize_tree(root);

	std::cout << "===== After inserting 20 =====" << std::endl;
	root->push_value(20, 20);
	print_tree_branch(root, 0);
	sanitize_tree(root);

	std::cout << "===== After inserting 40 =====" << std::endl;
	root->push_value(40, 40);
	print_tree_branch(root, 0);
	sanitize_tree(root);

	std::cout << "===== After inserting 70 =====" << std::endl;
	root->push_value(70, 70);
	print_tree_branch(root, 0);
	sanitize_tree(root);

	std::cout << "Value for " << k << ": " << root->find_value(k) << std::endl;

	for (int i = 0; i < 10; ++i) {
		int v = i + ((i % 2) ? 50 : 0) + 1;
		std::cout << "===== After inserting " << v << " =====" << std::endl;
		root->push_value(v, v);
		print_tree_branch(root, 0);
		sanitize_tree(root);
	}
	print_tree_branch(root, 0);

	std::cout << "===== Removing 50 =====" << std::endl;
	k = 50;
	root->remove_value(k);
	print_tree_branch(root, 0);
	sanitize_tree(root);

	std::cout << "===== Removing 70 =====" << std::endl;
	k = 70;
	root->remove_value(k);
	print_tree_branch(root, 0);
	sanitize_tree(root);

	std::cout << "===== Removing root =====" << std::endl;
	k = 30;
	root->remove_value(k);
	print_tree_branch(root, 0);
	sanitize_tree(root);
}

int main(void) {
	vectorTest();

	ft::stack<ValiditySanitizer> s;
	s.push(ValiditySanitizer(1));
	s.push(ValiditySanitizer(2));
	s.pop();
	s.pop();

	test_node();

	return 0;
}
