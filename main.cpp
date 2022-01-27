#include "iterator.hpp"
#include "type_traits.hpp"
#include "algorithm.hpp"
#include "utility.hpp"

#include "vector.hpp"

#include "stack.hpp"

#include "map.hpp"

#include <ostream>
#include <iostream>
#include <iomanip>

#include <typeinfo>

#include <vector>
#include <stack>
#include <map>
#include <iterator>
#include <utility>

#include <sstream>

#ifdef ULDL_DBG
#pragma message ("ULDL Enabled :)")

#include <uldl/uldl.hpp>

#define ULDL_INIT uldl::init();
#define ULDL_TRACKER(T, X) uldl::tracker<T> X;
#define ULDL_TRACKER_INIT(X) X(this),

#else

#define ULDL_INIT
#define ULDL_TRACKER(T, X)
#define ULDL_TRACKER_INIT(X)

#endif

#ifndef DSP_CHECK_VAL
# define DSP_CHECK_VAL true
#endif

#define VECTOR_TEST(X) X; \
						check_vector(validity, testIdx++)

#define MAP_OP(X) X; check_map(map.begin(), map.end());

#define CONSOLE_RED "\x1B[91m"
#define CONSOLE_GREEN "\x1B[92m"
#define CONSOLE_RESET "\x1B[0m"

#ifndef TEST_NAMESPACE
# define TEST_NAMESPACE ft
#endif

#define _SUB_MACRO_TO_STRING(X) #X
#define MACRO_TO_STRING(X) _SUB_MACRO_TO_STRING(X)

#ifdef SHOW_CAPACITY
# define DSP_CAPACITY(X) X
#else
# define DSP_CAPACITY(X) "ign_for_diff"
#endif

#ifdef SHOW_ORIGIN
# define DSP_ORIGIN(X) X
#else
# define DSP_ORIGIN(X) "ign_for_diff"
#endif

#define _SUB_MAKE_NAME(A, B) A##B
#define MAKE_NAME(A, B) _SUB_MAKE_NAME(A, B)

struct compare_expected {
	enum _compare_expected_v {
		EQUALS,
		GREATER,
		LOWER
	};
};

struct validity_sanitizer {
	enum origin_v {
		DFL,
		CTOR,
		CPY_CTOR,
		CPY_OP,
		DTOR,
	};

	ULDL_TRACKER(validity_sanitizer, tracker);

	bool valid;
	origin_v origin;
	std::size_t idx;

	char *mem_leak_check;

	validity_sanitizer(): ULDL_TRACKER_INIT(tracker) valid(true), origin(DFL), idx(-1), mem_leak_check(new char[1]) {}
	validity_sanitizer(size_t idx): ULDL_TRACKER_INIT(tracker) valid(true), origin(CTOR), idx(idx), mem_leak_check(new char[1]) {}
	validity_sanitizer(const validity_sanitizer &other): ULDL_TRACKER_INIT(tracker) valid(other.valid), origin(CPY_CTOR), idx(other.idx), mem_leak_check(new char[1]) {}

	~validity_sanitizer() {
		valid = false;
		origin = DTOR;
		delete[] mem_leak_check;
	}

	validity_sanitizer &operator=(const validity_sanitizer &lhs) {
		valid = lhs.valid;
		origin = CPY_OP;
		idx = lhs.idx;

		return (*this);
	}

	void check() const {
		if (!valid) {
			throw std::runtime_error((std::string("not valid object (") + DSP_ORIGIN(originToString(origin)) + ")").data());
		}
	}

	bool operator==(const validity_sanitizer &rhs) const {
		return (idx == rhs.idx);
	}

	bool operator<(const validity_sanitizer &rhs) const {
		return (idx < rhs.idx);
	}

	static std::string originToString(validity_sanitizer::origin_v origin) {
		switch (origin) {
		case DFL:		return "DFL";
		case CTOR:		return "CTOR";
		case CPY_CTOR:	return "CPY_CTOR";
		case CPY_OP:	return "CPY_OP";
		case DTOR:		return "DTOR";
		}
		return "UNKNOWN";
	}
};

std::ostream &operator<<(std::ostream &os, const validity_sanitizer::origin_v origin) {
	(void)origin;
	return (os << DSP_ORIGIN(validity_sanitizer::originToString(origin)));
}

std::ostream &operator<<(std::ostream &os, const validity_sanitizer &san) {
	return (os << "validity_sanitizer {idx: " << san.idx << ", origin: " << san.origin << ", valid: " << san.valid << "}");
}

template<typename _T0, typename _T1>
std::ostream &operator<<(std::ostream &os, const TEST_NAMESPACE::pair<_T0, _T1> &p) {
	return (os << " pair {first: " << p.first << ", second: " << p.second << "}");
}

template<typename _Iterator>
void test_iterator_if_rai(_Iterator it, std::random_access_iterator_tag) {
	std::cout << "[+] *(iterator += 2): " << *(it += 2) << std::endl;
	std::cout << "[+] *(iterator -= 2): " << *(it -= 2) << std::endl;
	std::cout << "[+] *(iterator + 2): " << *(it + 2) << std::endl;
	std::cout << "[+] *iterator: " << *it << std::endl;
	std::cout << "[+] *(2 + iterator): " << *(2 + it) << std::endl;
	std::cout << "[+] *iterator: " << *it << std::endl;
	std::cout << "[+] *(iterator - 2): " << *(it - 2) << std::endl;
	std::cout << "[+] (iterator - (iterator + 2)): " << (it - (it + 2)) << std::endl;
	std::cout << "[+] iterator[2]: " << it[2] << std::endl;
	std::cout << "[+] iterator[-2]: " << it[-2] << std::endl;
	std::cout << "[+] iterator < (iterator - 2): " << (it < (it - 2)) << std::endl;
	std::cout << "[+] iterator > (iterator - 2): " << (it > (it - 2)) << std::endl;
	std::cout << "[+] iterator <= (iterator - 2): " << (it <= (it - 2)) << std::endl;
	std::cout << "[+] iterator >= (iterator - 2): " << (it >= (it - 2)) << std::endl;
	std::cout << "[+] iterator < (iterator + 2): " << (it < (it + 2)) << std::endl;
	std::cout << "[+] iterator > (iterator + 2): " << (it > (it + 2)) << std::endl;
	std::cout << "[+] iterator <= (iterator + 2): " << (it <= (it + 2)) << std::endl;
	std::cout << "[+] iterator >= (iterator + 2): " << (it >= (it + 2)) << std::endl;
	std::cout << "[+] iterator < iterator: " << (it < it) << std::endl;
	std::cout << "[+] iterator > iterator: " << (it > it) << std::endl;
	std::cout << "[+] iterator <= iterator: " << (it <= it) << std::endl;
	std::cout << "[+] iterator >= iterator: " << (it >= it) << std::endl;
}

template<typename _Iterator>
void test_iterator_if_rai(_Iterator, std::input_iterator_tag) {}

template<typename _Iterator>
void test_iterator(_Iterator it) {
	const _Iterator &const_it = it;
	_Iterator copy = it;
	_Iterator default_constructible;

	std::cout << "[+] *iterator" << *it << std::endl;
	std::cout << "[+] *const_it" << *const_it << std::endl;
	std::cout << "[+] *(++iterator)" << *(++it) << std::endl;
	std::cout << "[+] iterator == iterator: " << (it == it) << std::endl;
	std::cout << "[+] iterator == copy: " << (it == copy) << std::endl;
	std::cout << "[+] iterator == const_it: " << (it == const_it) << std::endl;
	std::cout << "[+] iterator == default_constructible: " << (it == default_constructible) << std::endl;
	std::cout << "[+] const_it == const_it: " << (const_it == const_it) << std::endl;
	std::cout << "[+] const_it == copy: " << (const_it == copy) << std::endl;
	std::cout << "[+] const_it == default_constructible: " << (const_it == default_constructible) << std::endl;
	std::cout << "[+] default_constructible == default_constructible: " << (default_constructible == default_constructible) << std::endl;
	std::cout << "[+] *(iterator++)" << *(it++) << std::endl;
	std::cout << "[+] *(copy++)" << *(copy++) << std::endl;
	std::cout << "[+] *iterator" << *(it) << std::endl;
	std::cout << "[+] *copy" << *(copy) << std::endl;
	copy = it++;
	std::cout << "[+] *iterator" << *(it) << std::endl;
	std::cout << "[+] *assigned copy" << *(copy) << std::endl;
	std::cout << "[+] *(--iterator)" << *(--it) << std::endl;
	std::cout << "[+] *(iterator--)" << *(it--) << std::endl;
	std::cout << "[+] *(copy--)" << *(copy--) << std::endl;

	test_iterator_if_rai(it, typename _Iterator::iterator_category());
}

template<typename _Iterator>
void test_rbegin(TEST_NAMESPACE::reverse_iterator<_Iterator> it) {
	const TEST_NAMESPACE::reverse_iterator<_Iterator> &const_it = it;
	++it;
	std::cout << "[+] *(iterator.base()): " << *(it.base()) << std::endl;
	std::cout << "[+] *(const_it.base()): " << *(const_it.base()) << std::endl;

	std::cout << "[+] *(iterator.base()): " << *(it.base()) << std::endl;

	TEST_NAMESPACE::reverse_iterator<_Iterator> copy(it);
	std::cout << "[+] *(copy.base()): " << *(copy.base()) << std::endl;
	std::cout << "[+] *copy: " << *copy << std::endl;

	copy = ++it;
	std::cout << "[+] *(assigned.base()): " << *(copy.base()) << std::endl;
	std::cout << "[+] *assigned: " << *copy << std::endl;
}


void check_vector(const TEST_NAMESPACE::vector<validity_sanitizer> &validity, std::size_t testIdx) {
	typedef TEST_NAMESPACE::vector<validity_sanitizer>::const_iterator iter;

	std::size_t idx = 0;

	std::cout << "== CHECK " << testIdx << " (current size: " << validity.size() << " out of " << DSP_CAPACITY(validity.capacity()) << ") ==" << std::endl;
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

void insert_from_other_vector(TEST_NAMESPACE::vector<validity_sanitizer> &validity, std::size_t &testIdx) {
	TEST_NAMESPACE::vector<validity_sanitizer> validity1;


	for (std::size_t i = 0; i < 10; ++i) {
		validity1.push_back(validity_sanitizer(i + 1000));
	}

	VECTOR_TEST(validity.insert(validity.begin() + 9, validity1.begin(), validity1.end()));
}

void assign_from_other_vector(TEST_NAMESPACE::vector<validity_sanitizer> &validity, std::size_t &testIdx) {
	TEST_NAMESPACE::vector<validity_sanitizer> validity1;


	for (std::size_t i = 0; i < 20; ++i) {
		validity1.push_back(validity_sanitizer(i + 6000));
	}

	VECTOR_TEST(validity.assign(validity1.begin(), validity1.end()));
}

void swap_from_other_vector(TEST_NAMESPACE::vector<validity_sanitizer> &validity, std::size_t &testIdx) {
	TEST_NAMESPACE::vector<validity_sanitizer> validity1;

	for (std::size_t i = 0; i < 20; ++i) {
		validity1.push_back(validity_sanitizer(i + 10000));
	}

	VECTOR_TEST(validity.swap(validity1));

	for (std::size_t i = 0; i < 20; ++i) {
		validity1.push_back(validity_sanitizer(i + 10000 + 20));
	}

	VECTOR_TEST(TEST_NAMESPACE::swap(validity, validity1));
	VECTOR_TEST(TEST_NAMESPACE::swap(validity, validity1));
}

template<typename _Tp>
void exec_compare_vector(const TEST_NAMESPACE::vector<_Tp> &v0, const TEST_NAMESPACE::vector<_Tp> &v1, compare_expected::_compare_expected_v result) {
	if ((v0 == v1) == (result == compare_expected::EQUALS)) {
		std::cout << CONSOLE_GREEN << "== test passed :)" << CONSOLE_RESET << std::endl;
	} else {
		std::cout << CONSOLE_RED << "== test not passed :(" << CONSOLE_RESET << std::endl;
	}

	if ((v0 != v1) == (result != compare_expected::EQUALS)) {
		std::cout << CONSOLE_GREEN << "!= test passed :)" << CONSOLE_RESET << std::endl;
	} else {
		std::cout << CONSOLE_RED << "!= test not passed :(" << CONSOLE_RESET << std::endl;
	}

	if ((v0 <= v1) == (result != compare_expected::GREATER)) {
		std::cout << CONSOLE_GREEN << "<= test passed :)" << CONSOLE_RESET << std::endl;
	} else {
		std::cout << CONSOLE_RED << "<= test not passed :(" << CONSOLE_RESET << std::endl;
	}

	if ((v0 < v1) == (result == compare_expected::LOWER)) {
		std::cout << CONSOLE_GREEN << "< test passed :)" << CONSOLE_RESET << std::endl;
	} else {
		std::cout << CONSOLE_RED << "< test not passed :(" << CONSOLE_RESET << std::endl;
	}

	if ((v0 >= v1) == (result != compare_expected::LOWER)) {
		std::cout << CONSOLE_GREEN << ">= test passed :)" << CONSOLE_RESET << std::endl;
	} else {
		std::cout << CONSOLE_RED << ">= test not passed :(" << CONSOLE_RESET << std::endl;
	}

	if ((v0 > v1) == (result == compare_expected::GREATER)) {
		std::cout << CONSOLE_GREEN << "> test passed :)" << CONSOLE_RESET << std::endl;
	} else {
		std::cout << CONSOLE_RED << "> test not passed :(" << CONSOLE_RESET << std::endl;
	}
}

void compare_vector(TEST_NAMESPACE::vector<validity_sanitizer> &validity) {
	TEST_NAMESPACE::vector<validity_sanitizer> cmp(validity);

	std::cout << "*** Compare (equals) ***" << std::endl;
	exec_compare_vector(validity, cmp, compare_expected::EQUALS);

	std::cout << "*** Compare (greater) ***" << std::endl;
	validity.back().idx++;
	exec_compare_vector(validity, cmp, compare_expected::GREATER);

	std::cout << "*** Compare (lower) ***" << std::endl;
	validity.back().idx -= 2;
	exec_compare_vector(validity, cmp, compare_expected::LOWER);

	std::cout << "*** Compare (lower size) ***" << std::endl;
	validity.pop_back();
	exec_compare_vector(validity, cmp, compare_expected::LOWER);

	std::cout << "*** Compare (greater size) ***" << std::endl;
	validity.push_back(cmp.back());
	validity.push_back(validity.back());
	validity.back().idx--;
	exec_compare_vector(validity, cmp, compare_expected::GREATER);

	std::cout << "*** Compare (both empty) ***" << std::endl;
	validity.clear();
	cmp.clear();
	exec_compare_vector(validity, cmp, compare_expected::EQUALS);
}

void assignment_operator(TEST_NAMESPACE::vector<validity_sanitizer> &validity, std::size_t &testIdx) {
	TEST_NAMESPACE::vector<validity_sanitizer> v1;

	// Java-style
	for (int i = 0; i < 16; ++i) {
		v1.push_back(validity_sanitizer(i + 1000));
	}

	VECTOR_TEST(validity = v1);
}

void test_remaining_ctor(TEST_NAMESPACE::vector<validity_sanitizer> &validity) {
	TEST_NAMESPACE::vector<validity_sanitizer> v0(validity.begin(), validity.end());
	TEST_NAMESPACE::vector<validity_sanitizer> v1(10, validity_sanitizer(69420));

	std::cout << "**** iterator CTOR TEST **** (vvvvvv)" << std::endl;
	check_vector(v0, 0);

	std::cout << "**** count-val CTOR TEST **** (vvvvvv)" << std::endl;
	check_vector(v1, 0);
}

void vector_test() {
	typedef TEST_NAMESPACE::vector<validity_sanitizer>::reverse_iterator riter;
	TEST_NAMESPACE::vector<validity_sanitizer> validity;
	std::size_t testIdx = 0;

	for (std::size_t i = 0; i < 5; ++i) {
		validity.push_back(validity_sanitizer(i));
	}

	VECTOR_TEST();

	VECTOR_TEST(validity.insert(validity.begin() + 5, validity_sanitizer(666)));

	VECTOR_TEST(validity.insert(validity.begin() + 5, 6, validity_sanitizer(667)));

	VECTOR_TEST(validity.insert(validity.begin() + 2, 42, validity_sanitizer(69420)));

	VECTOR_TEST(validity.insert(validity.begin() + 2, 0, validity_sanitizer(99999)));

	VECTOR_TEST(insert_from_other_vector(validity, testIdx));

	VECTOR_TEST(validity.erase(validity.begin() + 5, validity.begin() + 10));

	VECTOR_TEST(validity.erase(validity.begin(), validity.begin()));

	VECTOR_TEST(validity.erase(validity.begin() + 5, validity.begin() + 5));

	VECTOR_TEST(validity.erase(validity.begin(), validity.end()));

	VECTOR_TEST(validity.erase(validity.begin(), validity.end()));

	for (std::size_t i = 0; i < 5; ++i) {
		validity.push_back(validity_sanitizer(i + 2000));
	}

	VECTOR_TEST();
	VECTOR_TEST(validity.clear());
	VECTOR_TEST(validity.clear());

	VECTOR_TEST(for (std::size_t i = 0; i < 20; ++i) {
		validity.push_back(validity_sanitizer(i + 3000));
	});

	VECTOR_TEST(while (!validity.empty()) {
		VECTOR_TEST(validity.pop_back());
	});

	VECTOR_TEST(for (std::size_t i = 0; i < 20; ++i) {
		validity.push_back(validity_sanitizer(i + 4000));
	});

	std::cout << "** test_iterator on begin() **" << std::endl;
	test_iterator(validity.begin() + 3);

	std::cout << "** test_iterator on rbegin() **" << std::endl;
	test_iterator(validity.rbegin() + 3);

	std::cout << "** test_rbegin **" << std::endl;
	test_rbegin(validity.rbegin());

	std::cout << "** output iterator test **" << std::endl;
	std::cout << "[+] insert at begin" << std::endl;
	{
		std::cout << "Before: " << *(validity.begin()) << std::endl;
		validity_sanitizer old = *(validity.begin());
		*(validity.begin()) = validity_sanitizer(-1);
		std::cout << "After: " << *(validity.begin()) << std::endl;
		VECTOR_TEST();
		*(validity.begin()) = old;
	}

	std::cout << "[+] insert at end - 1" << std::endl;
	{
		std::cout << "Before: " << *(validity.end() - 1) << std::endl;
		validity_sanitizer old = *(validity.end() - 1);
		*(validity.end() - 1) = validity_sanitizer(-1);
		std::cout << "After: " << *(validity.end() - 1) << std::endl;
		VECTOR_TEST();
		*(validity.end() - 1) = old;
	}

	std::cout << "[+] insert at rbegin" << std::endl;
	{
		std::cout << "Before: " << *(validity.rbegin()) << std::endl;
		validity_sanitizer old = *(validity.rbegin());
		*(validity.rbegin()) = validity_sanitizer(-1);
		std::cout << "After: " << *(validity.rbegin()) << std::endl;
		VECTOR_TEST();
		*(validity.rbegin()) = old;
	}

	std::cout << "[+] insert at rend - 1" << std::endl;
	{
		std::cout << "Before: " << *(validity.rend() - 1) << std::endl;
		validity_sanitizer old = *(validity.rend() - 1);
		*(validity.rend() - 1) = validity_sanitizer(-1);
		std::cout << "After: " << *(validity.rend() - 1) << std::endl;
		VECTOR_TEST();
		*(validity.rend() - 1) = old;
	}

	std::cout << "== REV 0 ==" << std::endl;
	VECTOR_TEST(for (riter it = validity.rbegin(); it != validity.rend(); ++it) {
		std::cout << "Rev'd " << (*it).idx << std::endl;
	});

	std::cout << "== REV 1 ==" << std::endl;
	VECTOR_TEST(for (riter it = validity.rbegin(); it != validity.rend(); ++it) {
		std::cout << "Rev'd " << it->idx << std::endl;
	});

	VECTOR_TEST(validity.assign(15, validity_sanitizer(5000)));
	VECTOR_TEST(assign_from_other_vector(validity, testIdx));
	VECTOR_TEST(validity.clear());
	assign_from_other_vector(validity, testIdx);

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
		std::cout << CONSOLE_RED << "/!\\ No exception caught (shit)" << CONSOLE_RESET << std::endl;
	} catch (std::exception &e) {
		std::cout << CONSOLE_GREEN << "/!\\ Caught an exception: " << e.what() << " (that's good)" << CONSOLE_RESET << std::endl;
	});

	VECTOR_TEST(std::cout << "Shitty vector test: " << (2 + validity.begin())->idx << std::endl);


	// Capacity, already tested......
	// Clear, already tested..................
	// Insert, already tested............................
	// Erase, already tested.........................................
	// Push_back, already tested.............................................
	// Pop_back, already tested.................................................

	VECTOR_TEST(validity.resize(15, validity_sanitizer(7000)));
	VECTOR_TEST(validity.resize(10, validity_sanitizer(7000)));
	VECTOR_TEST(validity.resize(16, validity_sanitizer(7000)));
	VECTOR_TEST(validity.resize(257, validity_sanitizer(8000)));
	VECTOR_TEST(swap_from_other_vector(validity, testIdx));

	VECTOR_TEST(compare_vector(validity));

	for (std::size_t i = 0; i < 20; ++i) {
		validity.push_back(validity_sanitizer(i));
	}

	VECTOR_TEST(assignment_operator(validity, testIdx));
	VECTOR_TEST(test_remaining_ctor(validity));
}

template<typename _Node>
void print_tree_branch(_Node *node, int depth) {
	for (int i = 0; i < depth; ++i) {
		std::cout << "          ";
	}

	std::cout << std::setw(4);

	if (!node || node->data == NULL) {
		std::cout << "       _" << std::endl;
		return;
	}

	std::cout << *(node->data) << " (" << (node->node_color == ft::__clsaad_impl::RED ? "R" : "B") << ")" << " ------- +" << std::endl;

	print_tree_branch(node->right_node, depth + 1);
	print_tree_branch(node->left_node, depth + 1);

}

template<typename _Node>
void sanitize_tree(_Node *root) {
	if (!root || root->data == NULL) {
		return;
	}

	if (root->left_node && root->left_node->data != NULL) {
		if (root->left_node->parent != root) {
			std::cerr << "Error at node " << *(root->left_node->data) << std::endl;
			throw std::runtime_error("TREE_SANITIZER: Wrong parent");
		}

		if (root->left_node->node_color == ft::__clsaad_impl::RED && root->left_node->node_color == root->node_color) {
			std::cerr << "Error at node " << *(root->left_node->data) << std::endl;
			throw std::runtime_error("TREE_SANITIZER: Wrong color");
		}
	}

	if (root->right_node && root->right_node->data != NULL) {
		if (root->right_node->parent != root) {
			std::cerr << "Error at node " << *(root->right_node->data) << std::endl;
			throw std::runtime_error("TREE_SANITIZER: Wrong parent");
		}

		if (root->right_node->node_color == ft::__clsaad_impl::RED && root->right_node->node_color == root->node_color) {
			std::cerr << "Error at node " << *(root->right_node->data) << std::endl;
			throw std::runtime_error("TREE_SANITIZER: Wrong color");
		}
	}

	sanitize_tree(root->left_node);
	sanitize_tree(root->right_node);
}

void test_tree() {
	typedef ft::__clsaad_impl::bst_wrapper< int, std::less<int> > bst_wrapper;
	int v;

	std::cout << "Constructing bst_wrapper..." << std::endl;
	bst_wrapper bst = bst_wrapper(std::less<int>(), std::allocator<int>());
	sanitize_tree(*bst.get_root());


	// int v = 50;
	// std::cout << "===== After inserting " << v << "=====" << std::endl;
	// bst.insert(v);
	// print_tree_branch(*bst.get_root(), 0);
	// sanitize_tree(*bst.get_root());

	v = 5;
	std::cout << "===== After inserting " << v << "=====" << std::endl;
	bst.insert(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());

	v = 30;
	std::cout << "===== After inserting " << v << "=====" << std::endl;
	bst.insert(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());

	v = 20;
	std::cout << "===== After inserting " << v << "=====" << std::endl;
	bst.insert(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());

	v = 40;
	std::cout << "===== After inserting " << v << "=====" << std::endl;
	bst.insert(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());

	v = 70;
	std::cout << "===== After inserting " << v << "=====" << std::endl;
	bst.insert(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());

	std::cout << "Value for " << v << ": " << *(bst.find_node(v).data) << std::endl;

	for (int i = 0; i < 10; ++i) {
		int v = i + ((i % 2) ? 50 : 0) + 1;
		std::cout << "===== After inserting " << v << " =====" << std::endl;
		bst.insert(v);
		sanitize_tree(*bst.get_root());
		print_tree_branch(*bst.get_root(), 0);
	}
	print_tree_branch(*bst.get_root(), 0);

	v = 40;
	std::cout << "===== Removing " << v << " =====" << std::endl;
	bst.erase(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());

	v = 70;
	std::cout << "===== Removing " << v << "=====" << std::endl;
	bst.erase(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());

	v = 30;
	std::cout << "===== Removing " << v << " =====" << std::endl;
	bst.erase(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());

	v = 52;
	std::cout << "===== Removing " << v << " =====" << std::endl;
	bst.erase(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());

	v = 42;
	std::cout << "===== Inserting " << v << " =====" << std::endl;
	bst.insert(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());

	v = 42;
	std::cout << "===== Removing " << v << " =====" << std::endl;
	bst.erase(v);
	print_tree_branch(*bst.get_root(), 0);
	sanitize_tree(*bst.get_root());
}

template<typename _Iterator>
void display_map(_Iterator begin, _Iterator end, std::string name = "map") {
	for (_Iterator it = begin; it != end; ++it) {
		std::cout << name << "[" << it->first << "] = " << it->second << std::endl;
	}
}

template<typename _It>
void check_map(_It it, _It end) {
	for (std::size_t idx = 0; it != end; ++idx, ++it) {
		if (DSP_CHECK_VAL) {
			if (idx < 31) {
				std::cout << "Checking element " << idx << " = " << it->second.idx << std::endl;
			} else if (idx == 31) {
				std::cout << "(...)" << std::endl;
			}
		}

		try {
			it->second.check();
		} catch (std::exception &exc) {
			std::cout << "Validity check failed on element number " << idx << " (key: " << it->first << ", mapped: " << it->second << ")" << std::endl;
			throw exc;
		}
	}
}

template<typename _Iterator, typename _Map>
void preserving_map_test(_Map &map) {
	typedef _Map map_type;
	typedef _Iterator iterator;
	// typedef typename map_type::allocator_type allocator_type;

	std::cout << "** test end - 1 iterator ** " << std::endl;

	// Otherwise, it's undefined behavior
	if (map.begin() != map.end()) {
		iterator it = map.end();
		--it;
		std::cout << "map[" << it->first << "] = " << it->second << std::endl;
	}

	std::cout << "** copy of map ** " << std::endl;
	{
		map_type copy(map);

		display_map(copy.begin(), copy.end(), "copy");
	}

	std::cout << "** test post copy destruction ** " << std::endl;
	display_map(map.begin(), map.end());

	std::cout << "** test copy of [begin + 1, end] ** " << std::endl;
	// Otherwise, it's undefined behavior
	if (map.begin() != map.end()) {
		map_type copy(++(map.begin()), map.end());

		display_map(copy.begin(), copy.end(), "copy");
	}

	std::cout << "** test post copy destruction ** " << std::endl;
	display_map(map.begin(), map.end());

	std::cout << "** test copy of [begin, end-1] ** " << std::endl;
	// Otherwise, it's undefined behavior
	if (map.begin() != map.end()) {
		map_type copy(map.begin(), --(map.end()));

		display_map(copy.begin(), copy.end(), "copy");
	}

	std::cout << "** test post copy destruction ** " << std::endl;
	display_map(map.begin(), map.end());

	std::cout << "** test for get_allocator ** " << std::endl;
	{
		map.get_allocator();
	}

	std::cout << "** test for at ** " << std::endl;
	for (iterator it = map.begin(); it != map.end(); ++it) {
		std::cout << "map.at(\"" << it->first << "\") = " << map.at(it->first) << std::endl;
	}

	std::cout << "** named tests ** " << std::endl;
	std::cout << "map.at with non-existing key... ";

	try {
		map.at("MDR NICE TRY");

		std::cout << CONSOLE_RED << "FAILED (nothing thrown)" << CONSOLE_RESET << std::endl;
	} catch (std::out_of_range &oor) {
		std::cout << CONSOLE_GREEN << "SUCCESSFUL (std::out_of_range exception thrown. YAY)" << CONSOLE_RESET << std::endl;
	} catch (std::exception &exc) {
		std::cout << CONSOLE_RED << "FAILED (wrong exception thrown, expected std::out_of_range, found " << typeid(exc).name() << ")" << CONSOLE_RESET << std::endl;
	} catch (...) {
		std::cout << CONSOLE_RED << "FAILED (IT'S NOT EVEN AN EXCEPTION. WHAT THE HELL IS WRONG WITH YOU)" << CONSOLE_RESET << std::endl;
	}


	std::cout << "** test for count ** " << std::endl;
	{
		std::size_t count;

		count = map.count("Bonchouuur0");
		std::cout << "existing key: " << (count == 1 ? CONSOLE_GREEN : CONSOLE_RED) << count << CONSOLE_RESET << std::endl;
		count = map.count("Bonchouuurfnjdk");
		std::cout << "non-existing key: " << (count == 0 ? CONSOLE_GREEN : CONSOLE_RED) << count << CONSOLE_RESET << std::endl;

	}

	std::cout << "** test for find ** " << std::endl;
	{
		_Iterator find_res;

		if (!map.empty()) {
			std::cout << "existing key: ";
			find_res = map.find("Bonchouuur0");
			if (find_res == map.end()) {
				std::cout << CONSOLE_RED << "not found!" << CONSOLE_RESET << std::endl;
			} else {
				std::cout << CONSOLE_GREEN << "found" << CONSOLE_RESET << ": " << "iterator {key: " << find_res->first << ", value: " << find_res->second << "}" << std::endl;
			}
		}

		std::cout << "non-existing key: ";
		find_res = map.find("fejdsfnel");

		if (find_res == map.end()) {
			std::cout << CONSOLE_GREEN << "not found!" << CONSOLE_RESET << std::endl;
		} else {
			std::cout << CONSOLE_RED << "found" << CONSOLE_RESET << ": " << "iterator {key: " << find_res->first << ", value: " << find_res->second << "}" << std::endl;
		}
	}

	std::cout << "** test for equal_range **" << std::endl;
	{
		TEST_NAMESPACE::pair<iterator, iterator> equal_range_res;

		if (!map.empty()) {
			std::cout << "existing key: " << std::endl;
			equal_range_res = map.equal_range("Bonchouuur1");
			std::cout << "\tfirst: " << "iterator {key: " << equal_range_res.first->first << std::flush << ", value: " << equal_range_res.first->second << "}" << std::endl;
			std::cout << "\tsecond: " << "iterator {key: " << equal_range_res.second->first << std::flush << ", value: " << equal_range_res.second->second << "}" << std::endl;
		}

		std::cout << "non-existing key: " << std::endl;
		equal_range_res = map.equal_range("Bonchouuur5");
		if (equal_range_res.first != map.end() && equal_range_res.second != map.end()) {
			std::cout << "\tfirst: " << "iterator {key: " << equal_range_res.first->first << ", value: " << equal_range_res.first->second << "}" << std::endl;
			std::cout << "\tsecond: " << "iterator {key: " << equal_range_res.second->first << ", value: " << equal_range_res.second->second << "}" << std::endl;
		} else {
			std::cout << "* returned [end(), end()] *" << std::endl;
		}

		std::cout << "non-existing past-the-end key: " << std::endl;
		equal_range_res = map.equal_range("Bonchouuur7");
		if (equal_range_res.first != map.end() && equal_range_res.second != map.end()) {
			std::cout << "\tfirst: " << "iterator {key: " << equal_range_res.first->first << ", value: " << equal_range_res.first->second << "}" << std::endl;
			std::cout << "\tsecond: " << "iterator {key: " << equal_range_res.second->first << ", value: " << equal_range_res.second->second << "}" << std::endl;
		} else {
			std::cout << "* returned [end(), end()] *" << std::endl;
		}
	}

	std::cout << "** test for lower_bound **" << std::endl;
	{
		iterator lower_bound_res;

		if (!map.empty()) {
			std::cout << "existing key: ";
			lower_bound_res = map.lower_bound("Bonchouuur1");
			std::cout << "iterator {key: " << lower_bound_res->first << std::flush << ", value: " << lower_bound_res->second << "}" << std::endl;
		}

		std::cout << "non-existing key: ";
		lower_bound_res = map.lower_bound("Bonchouuur5");
		if (lower_bound_res != map.end() && lower_bound_res != map.end()) {
			std::cout << "iterator {key: " << lower_bound_res->first << ", value: " << lower_bound_res->second << "}" << std::endl;
		} else {
			std::cout << "* returned [end(), end()] *" << std::endl;
		}

		std::cout << "non-existing past-the-end key: ";
		lower_bound_res = map.lower_bound("Bonchouuur7");
		if (lower_bound_res != map.end() && lower_bound_res != map.end()) {
			std::cout << "iterator {key: " << lower_bound_res->first << ", value: " << lower_bound_res->second << "}" << std::endl;
		} else {
			std::cout << "* returned [end(), end()] *" << std::endl;
		}
	}

	std::cout << "** test for upper_bound **" << std::endl;
	{
		iterator upper_bound_res;

		if (!map.empty()) {
			std::cout << "existing key: ";
			upper_bound_res = map.upper_bound("Bonchouuur1");
			std::cout << "iterator {key: " << upper_bound_res->first << std::flush << ", value: " << upper_bound_res->second << "}" << std::endl;
		}

		std::cout << "non-existing key: ";
		upper_bound_res = map.upper_bound("Bonchouuur5");
		if (upper_bound_res != map.end() && upper_bound_res != map.end()) {
			std::cout << "iterator {key: " << upper_bound_res->first << ", value: " << upper_bound_res->second << "}" << std::endl;
		} else {
			std::cout << "* returned [end(), end()] *" << std::endl;
		}

		std::cout << "non-existing past-the-end key: ";
		upper_bound_res = map.upper_bound("Bonchouuur7");
		if (upper_bound_res != map.end() && upper_bound_res != map.end()) {
			std::cout << "iterator {key: " << upper_bound_res->first << ", value: " << upper_bound_res->second << "}" << std::endl;
		} else {
			std::cout << "* returned [end(), end()] *" << std::endl;
		}
	}


	std::cout << "** reverse iterator test **" << std::endl;
	display_map(map.rbegin(), map.rend());

	std::cout << "is map empty: " << ((map.empty() == (map.size() == 0)) ? CONSOLE_GREEN : CONSOLE_RED) << (map.empty() ? "yes" : "no") << CONSOLE_RESET << std::endl;

	std::cout << "** key_comp test **" << std::endl;
	typename map_type::key_compare key_comp = map.key_comp();
	if (key_comp("0", "1"))
		std::cout << "works? " << CONSOLE_GREEN << "yes" << CONSOLE_RESET << std::endl;
	else
		std::cout << "works? " << CONSOLE_RED << "no" << CONSOLE_RESET << std::endl;

	std::cout << "** value_comp test **" << std::endl;
	typename map_type::value_compare value_comp = map.value_comp();
	if (value_comp(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("0", validity_sanitizer(-1)), TEST_NAMESPACE::pair<std::string, validity_sanitizer>("1", validity_sanitizer(-1))))
		std::cout << "works? " << CONSOLE_GREEN << "yes" << CONSOLE_RESET << std::endl;
	else
		std::cout << "works? " << CONSOLE_RED << "no" << CONSOLE_RESET << std::endl;


	std::cout << "** comparing operators test **" << std::endl;
	{
		TEST_NAMESPACE::map<std::string, validity_sanitizer> _smaller;
		TEST_NAMESPACE::map<std::string, validity_sanitizer> _just_smaller;
		TEST_NAMESPACE::map<std::string, validity_sanitizer> _just_bigger;
		TEST_NAMESPACE::map<std::string, validity_sanitizer> _bigger;
		TEST_NAMESPACE::map<std::string, validity_sanitizer> _nearly_totally_different;
		TEST_NAMESPACE::map<std::string, validity_sanitizer> _totally_different;

		_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur ", validity_sanitizer(-1)));
		_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur/", validity_sanitizer(-1)));
		_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur0", validity_sanitizer(-1)));
		_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur1", validity_sanitizer(-1)));
		_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur2", validity_sanitizer(-1)));

		_just_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur ", validity_sanitizer(-1)));
		_just_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur/", validity_sanitizer(-1)));
		_just_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur0", validity_sanitizer(-1)));
		_just_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur1", validity_sanitizer(-1)));
		_just_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur2", validity_sanitizer(-1)));
		_just_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur3", validity_sanitizer(-1)));
		_just_smaller.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur4", validity_sanitizer(-1)));

		_just_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur ", validity_sanitizer(-1)));
		_just_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur/", validity_sanitizer(-1)));
		_just_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur0", validity_sanitizer(-1)));
		_just_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur1", validity_sanitizer(-1)));
		_just_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur2", validity_sanitizer(-1)));
		_just_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur3", validity_sanitizer(-1)));
		_just_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur4", validity_sanitizer(-1)));
		_just_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur6", validity_sanitizer(-1)));
		_just_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur7", validity_sanitizer(-1)));

		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur ", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur/", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur0", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur1", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur2", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur3", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur4", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur6", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur7", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur8", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur9", validity_sanitizer(-1)));
		_bigger.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur:", validity_sanitizer(-1)));

		_nearly_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("frofbsjl", validity_sanitizer(-1)));
		_nearly_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("52014520", validity_sanitizer(-1)));
		_nearly_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("Bonchouuur0", validity_sanitizer(-1)));
		_nearly_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("-09876545678_)(", validity_sanitizer(-1)));
		_nearly_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("(*&^%^&*())", validity_sanitizer(-1)));
		_nearly_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("JJHJBHJBN", validity_sanitizer(-1)));
		_nearly_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("JJHJBHJBNfjrk", validity_sanitizer(-1)));
		_nearly_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("ghirufghh", validity_sanitizer(-1)));

		_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("frofbsjl", validity_sanitizer(-1)));
		_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("52014520", validity_sanitizer(-1)));
		_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("fjeiofhoehfoejfh", validity_sanitizer(-1)));
		_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("-09876545678_)(", validity_sanitizer(-1)));
		_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("(*&^%^&*())", validity_sanitizer(-1)));
		_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("JJHJBHJBN", validity_sanitizer(-1)));
		_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("JJHJBHJBNfjrk", validity_sanitizer(-1)));
		_totally_different.insert(TEST_NAMESPACE::pair<std::string, validity_sanitizer>("ghirufghh", validity_sanitizer(-1)));

		// Yeah, weird-ass compiler ain't capable to find the right if it's not const *smh*
		const TEST_NAMESPACE::map<std::string, validity_sanitizer> &smaller = _smaller;
		const TEST_NAMESPACE::map<std::string, validity_sanitizer> &just_smaller = _just_smaller;
		const TEST_NAMESPACE::map<std::string, validity_sanitizer> &just_bigger = _just_bigger;
		const TEST_NAMESPACE::map<std::string, validity_sanitizer> &bigger = _bigger;
		const TEST_NAMESPACE::map<std::string, validity_sanitizer> &nearly_totally_different = _nearly_totally_different;
		const TEST_NAMESPACE::map<std::string, validity_sanitizer> &totally_different = _totally_different;

		std::cout << "[+] operator==" << std::endl;

		std::cout << "Same input: ";
		if (map == map)
			std::cout << CONSOLE_GREEN << "equal" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_RED << "different" << CONSOLE_RESET << std::endl;

		std::cout << "Smaller input: ";
		if (map == smaller)
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;

		std::cout << "Just smaller input: ";
		if (map == just_smaller)
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;

		std::cout << "Just bigger input: ";
		if (map == just_bigger)
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;

		std::cout << "Bigger input: ";
		if (map == bigger)
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;

		std::cout << "Nearly totally different input: ";
		if (map == nearly_totally_different)
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;

		std::cout << "Totally different input: ";
		if (map == totally_different)
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;


		std::cout << "[+] operator!=" << std::endl;

		std::cout << "Same input: ";
		if (map != map)
			std::cout << CONSOLE_RED << "different" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_GREEN << "equal" << CONSOLE_RESET << std::endl;

		std::cout << "Smaller input: ";
		if (map != smaller)
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;

		std::cout << "Just smaller input: ";
		if (map != just_smaller)
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;

		std::cout << "Just bigger input: ";
		if (map != just_bigger)
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;

		std::cout << "Bigger input: ";
		if (map != bigger)
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;

		std::cout << "Nearly totally different input: ";
		if (map != nearly_totally_different)
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;

		std::cout << "Totally different input: ";
		if (map != totally_different)
			std::cout << CONSOLE_GREEN << "different" << CONSOLE_RESET << std::endl;
		else
			std::cout << CONSOLE_RED << "equal" << CONSOLE_RESET << std::endl;

		std::cout << "[+] operator<" << std::endl;
		std::cout << "Same input: " << (map < map ? "not " : "") << "less" << std::endl;
		std::cout << "Smaller input: " << (map < smaller ? "not " : "") << "less" << std::endl;
		std::cout << "Just smaller input: " << (map < just_smaller ? "not " : "") << "less" << std::endl;
		std::cout << "Just bigger input: " << (map < just_bigger ? "not " : "") << "less" << std::endl;
		std::cout << "Bigger input: " << (map < bigger ? "not " : "") << "less" << std::endl;
		std::cout << "Nearly totally different input: " << (map < nearly_totally_different ? "not " : "") << "less" << std::endl;
		std::cout << "Totally different input: " << (map < totally_different ? "not " : "") << "less" << std::endl;

		std::cout << "[+] operator<=" << std::endl;
		std::cout << "Same input: " << (map <= map ? "not " : "") << "less" << std::endl;
		std::cout << "Smaller input: " << (map <= smaller ? "not " : "") << "less" << std::endl;
		std::cout << "Just smaller input: " << (map <= just_smaller ? "not " : "") << "less" << std::endl;
		std::cout << "Just bigger input: " << (map <= just_bigger ? "not " : "") << "less" << std::endl;
		std::cout << "Bigger input: " << (map <= bigger ? "not " : "") << "less" << std::endl;
		std::cout << "Nearly totally different input: " << (map <= nearly_totally_different ? "not " : "") << "less" << std::endl;
		std::cout << "Totally different input: " << (map <= totally_different ? "not " : "") << "less" << std::endl;

		std::cout << "[+] operator>" << std::endl;
		std::cout << "Same input: " << (map > map ? "not " : "") << "less" << std::endl;
		std::cout << "Smaller input: " << (map > smaller ? "not " : "") << "less" << std::endl;
		std::cout << "Just smaller input: " << (map > just_smaller ? "not " : "") << "less" << std::endl;
		std::cout << "Just bigger input: " << (map > just_bigger ? "not " : "") << "less" << std::endl;
		std::cout << "Bigger input: " << (map > bigger ? "not " : "") << "less" << std::endl;
		std::cout << "Nearly totally different input: " << (map > nearly_totally_different ? "not " : "") << "less" << std::endl;
		std::cout << "Totally different input: " << (map > totally_different ? "not " : "") << "less" << std::endl;

		std::cout << "[+] operator>=" << std::endl;
		std::cout << "Same input: " << (map >= map ? "not " : "") << "less" << std::endl;
		std::cout << "Smaller input: " << (map >= smaller ? "not " : "") << "less" << std::endl;
		std::cout << "Just smaller input: " << (map >= just_smaller ? "not " : "") << "less" << std::endl;
		std::cout << "Just bigger input: " << (map >= just_bigger ? "not " : "") << "less" << std::endl;
		std::cout << "Bigger input: " << (map >= bigger ? "not " : "") << "less" << std::endl;
		std::cout << "Nearly totally different input: " << (map >= nearly_totally_different ? "not " : "") << "less" << std::endl;
		std::cout << "Totally different input: " << (map >= totally_different ? "not " : "") << "less" << std::endl;

	}
}

void clear_map_test(TEST_NAMESPACE::map<std::string, validity_sanitizer> &map) {
	bool already_failed = false;
	map.clear();
	if (map.begin() != map.end()) {
		std::cout << CONSOLE_RED << "Iterator failed *begin() != end()*";
		already_failed = true;
	}

	if (!map.empty()) {
		if (already_failed) {
			std::cout << ", ";
		}
		std::cout << CONSOLE_RED << " empty fail *empty() != true*";
		already_failed = true;
	}

	if (map.size() != 0) {
		if (already_failed) {
			std::cout << ", ";
		}
		std::cout << CONSOLE_RED << " empty fail *empty() != true*";
		already_failed = true;
	}

	if (!already_failed) {
		std::cout << CONSOLE_GREEN << "OKAY :)";
	}

	std::cout << CONSOLE_RESET << std::endl;
}

template<typename _Map, typename _Key, typename _Value>
void insert_test(_Map &map, const _Key &key, const _Value &value, bool should_succeed) {
	typedef typename _Map::iterator it;

	TEST_NAMESPACE::pair<it, bool> insert_res = map.insert(typename _Map::value_type(key, value));
	std::cout << "return value: "
		<< "iterator {key: " << insert_res.first->first << ", value: " << insert_res.first->second << "}, "
		<< "successful: "
			<< (insert_res.second == should_succeed ? CONSOLE_GREEN : CONSOLE_RED)
			<< (insert_res.second ? "yes" : "no")
	<< CONSOLE_RESET << std::endl;

	std::cout << "map content: " << std::endl;
	display_map(map.begin(), map.end());
}

template<typename _Map, typename _Key, typename _Value>
void insert_test(_Map &map, const _Key &key, const _Value &value, typename _Map::iterator hint) {
	typedef typename _Map::iterator it;

	it insert_res = map.insert(hint, typename _Map::value_type(key, value));
	std::cout << "return value: "
		<< "iterator {key: " << insert_res->first << ", value: " << insert_res->second << "}"
	<< std::endl;

	std::cout << "map content: " << std::endl;
	display_map(map.begin(), map.end());
}

void test_map() {
	typedef TEST_NAMESPACE::map<std::string, validity_sanitizer> map_type;

	map_type map;
	std::size_t idx = 0;

	std::cout << "======= MAP - ALTERING TESTS =======" << std::endl;

	std::cout << "** Basic insert on empty map **" << std::endl;
	map.insert(map_type::value_type("Bonchouuur0", validity_sanitizer(idx++)));

	for (map_type::iterator it = map.begin(); it != map.end(); ++it) {
		std::cout << "map[" << it->first << "] = " << it->second << std::endl;
	}
	check_map(map.begin(), map.end());


	std::cout << "** Basic insert on map with size == 1 **" << std::endl;
	map.insert(map_type::value_type("Bonchouuur1", validity_sanitizer(idx++)));

	for (map_type::iterator it = map.begin(); it != map.end(); ++it) {
		std::cout << "map[" << it->first << "] = " << it->second << std::endl;
	}

	check_map(map.begin(), map.end());

	std::cout << "** 2 consecutive basic inserts on map with size > 1 **" << std::endl;
	map.insert(map_type::value_type("Bonchouuur3", validity_sanitizer(idx++ + 1)));
	map.insert(map_type::value_type("Bonchouuur2", validity_sanitizer(idx++ - 1)));

	for (map_type::iterator it = map.begin(); it != map.end(); ++it) {
		std::cout << "map[" << it->first << "] = " << it->second << std::endl;
	}

	std::cout << "** operator[] test **" << std::endl;
	std::cout << "operator[] with 1st element = " << map["Bonchouuur0"] << std::endl;
	std::cout << "operator[] with 2nd element = " << map["Bonchouuur1"] << std::endl;
	std::cout << "operator[] with 4th element = " << map["Bonchouuur3"] << std::endl;
	std::cout << "operator[] with non-existent element (biggest before beginning of the map) = " << map["Bonchouuur/"] << std::endl;
	std::cout << "operator[] with non-existent element (gap from beginning) = " << map["Bonchouuur "] << std::endl;
	std::cout << "operator[] with non-existent element (lowest after end of the map) = " << map["Bonchouuur4"] << std::endl;
	std::cout << "operator[] with non-existent element (gap from the end) = " << map["Bonchouuur6"] << std::endl;

	std::cout << "** test_iterator on begin() **" << std::endl;
	test_iterator(++++++(map.begin()));

	std::cout << "** test_iterator on rbegin() **" << std::endl;
	test_iterator(++++++(map.rbegin()));

	std::cout << "** test_rbegin **" << std::endl;
	test_rbegin(map.rbegin());

	std::cout << "** output iterator test **" << std::endl;
	std::cout << "[+] insert at begin" << std::endl;
	{
		std::cout << "Before: " << *(map.begin()) << std::endl;
		validity_sanitizer old = map.begin()->second;
		map.begin()->second = validity_sanitizer(-1);
		std::cout << "After: " << *(map.begin()) << std::endl;
		(*(map.begin())).second = old;
		std::cout << "Reset: " << *(map.begin()) << std::endl;
	}

	std::cout << "[+] insert at --end" << std::endl;
	{
		std::cout << "Before: " << *(--map.end()) << std::endl;
		validity_sanitizer old = (--map.end())->second;
		(--map.end())->second = validity_sanitizer(-1);
		std::cout << "After: " << *(--map.end()) << std::endl;
		(*(--map.end())).second = old;
		std::cout << "Reset: " << *(--map.end()) << std::endl;
	}

	std::cout << "[+] insert at rbegin" << std::endl;
	{
		std::cout << "Before: " << *(map.rbegin()) << std::endl;
		validity_sanitizer old = map.rbegin()->second;
		map.rbegin()->second = validity_sanitizer(-1);
		std::cout << "After: " << *(map.rbegin()) << std::endl;
		(*(map.rbegin())).second = old;
		std::cout << "Reset: " << *(map.rbegin()) << std::endl;
	}

	std::cout << "[+] insert at --rend" << std::endl;
	{
		std::cout << "Before: " << *(--map.rend()) << std::endl;
		validity_sanitizer old = (--map.rend())->second;
		(--map.rend())->second = validity_sanitizer(-1);
		std::cout << "After: " << *(--map.rend()) << std::endl;
		(*(--map.rend())).second = old;
		std::cout << "Reset: " << *(--map.rend()) << std::endl;
	}

	for (map_type::iterator it = map.begin(); it != map.end(); ++it) {
		std::cout << "map[" << it->first << "] = " << it->second << std::endl;
	}

	std::cout << "======= MAP - PRESERVING TESTS (on non-const reference) =======" << std::endl;
	preserving_map_test<map_type::iterator>(map);
	std::cout << "========= MAP - PRESERVING TESTS (on const reference) =========" << std::endl;
	preserving_map_test<map_type::const_iterator, const map_type>(map);
	std::cout << "================ END OF MAP - PRESERVING TESTS ================" << std::endl;



	std::cout << "** clear test **" << std::endl;
	std::cout << "On non-empty map... " << std::flush;
	clear_map_test(map);
	std::cout << "On empty map... " << std::flush;
	clear_map_test(map);

	std::cout << "===== EMPTY MAP - PRESERVING TESTS (on non-const reference) ====" << std::endl;
	preserving_map_test<map_type::iterator>(map);
	std::cout << "======= EMPTY MAP - PRESERVING TESTS (on const reference) ======" << std::endl;
	preserving_map_test<map_type::const_iterator, const map_type>(map);
	std::cout << "============== END OF EMPTY MAP - PRESERVING TESTS =============" << std::endl;

	std::cout << "** insert test post clear **" << std::endl;

	std::cout << "[+] insert on empty map..." << std::endl;
	insert_test(map, "val_san0", validity_sanitizer(idx++), true);

	std::cout << "[+] insert already existing key (size = 1)..." << std::endl;
	insert_test(map, "val_san0", validity_sanitizer(idx), false);

	std::cout << "[+] insert on non-empty map (size = 1)..." << std::endl;
	insert_test(map, "val_san1", validity_sanitizer(idx++), true);

	std::cout << "[+] insert already existing key (size = 2)..." << std::endl;
	insert_test(map, "val_san1", validity_sanitizer(idx), false);

	{
		std::cout << "[+] insert from iterators" << std::endl;
		// Yep. Using the standard vector to fill it, just so we can confirm it works
		std::vector< TEST_NAMESPACE::pair<std::string, validity_sanitizer> > pairs;

		for (int i = 0; i < 8; ++i) {
			std::stringstream ss;

			ss << "val_san" << (i + 4);
			pairs.push_back(TEST_NAMESPACE::make_pair(ss.str(), validity_sanitizer((idx++) + i)));
		}

		map.insert(pairs.begin(), pairs.end());
		display_map(map.begin(), map.end());
	}

	std::cout << "[+] insert with hint next to expected place..." << std::endl;
	insert_test(map, "val_san2", validity_sanitizer(idx++), ++++(map.begin()));

	std::cout << "[+] insert with hint away to expected place..." << std::endl;
	insert_test(map, "val_san3", validity_sanitizer(idx++), map.begin());

	std::cout << "** erase test **" << std::endl;
	std::cout << "[+] insert with hint away to expected place..." << std::endl;
	insert_test(map, "val_san3", validity_sanitizer(idx++), map.begin());

	std::cout << "[+] erase iterator 0" << std::endl;
	map.erase(map.begin());
	display_map(map.begin(), map.end());
	check_map(map.begin(), map.end());

	std::cout << "[+] erase iterator 1" << std::endl;
	map.erase(++map.begin());
	display_map(map.begin(), map.end());
	check_map(map.begin(), map.end());

	std::cout << "[+] erase range" << std::endl;
	map.erase(++map.begin(), ++++map.begin());
	display_map(map.begin(), map.end());
	check_map(map.begin(), map.end());

	std::cout << "[+] erase range (end, end)" << std::endl;
	map.erase(map.end(), map.end());
	display_map(map.begin(), map.end());
	check_map(map.begin(), map.end());

	std::cout << "[+] erase key" << std::endl;
	std::cout << "delete existing key: " << map.erase("val_san9") << std::endl;
	std::cout << "delete previously-existing key: " << map.erase("val_san9") << std::endl;
	std::cout << "delete non-existing key: " << map.erase("val_sanJfkdfklenfjkn") << std::endl;

	std::cout << "** swap test **" << std::endl;
	{
		map_type::value_type pair[5] = {
			map_type::value_type("swap_vsan0", validity_sanitizer(0)),
			map_type::value_type("swap_vsan1", validity_sanitizer(1)),
			map_type::value_type("swap_vsan2", validity_sanitizer(2)),
			map_type::value_type("swap_vsan3", validity_sanitizer(3)),
			map_type::value_type("swap_vsan4", validity_sanitizer(4)),
		};
		map_type other(&pair[0], &pair[5]);

		std::cout << "[+] member swap" << std::endl;
		std::cout << "Main map (before swap)" << std::endl;
		display_map(map.begin(), map.end());
		check_map(map.begin(), map.end());

		std::cout << "Other map (before swap)" << std::endl;
		display_map(other.begin(), other.end());
		check_map(other.begin(), other.end());

		map.swap(other);

		std::cout << "Main map (after swap)" << std::endl;
		display_map(map.begin(), map.end());
		check_map(map.begin(), map.end());

		std::cout << "Other map (after swap)" << std::endl;
		display_map(other.begin(), other.end());
		check_map(other.begin(), other.end());

		std::cout << "[+] non-member swap" << std::endl;
		std::cout << "Main map (before swap)" << std::endl;
		display_map(map.begin(), map.end());
		check_map(map.begin(), map.end());

		std::cout << "Other map (before swap)" << std::endl;
		display_map(other.begin(), other.end());
		check_map(other.begin(), other.end());

		swap(other, map);

		std::cout << "Main map (after swap)" << std::endl;
		display_map(map.begin(), map.end());
		check_map(map.begin(), map.end());

		std::cout << "Other map (after swap)" << std::endl;
		display_map(other.begin(), other.end());
		check_map(other.begin(), other.end());
	}

	std::cout << "** Constructors tests **" << std::endl;
	std::cout << "[+] copy constructor" << std::endl;
	{
		std::cout << "Original" << std::endl;
		display_map(map.begin(), map.end());

		std::cout << "Copy" << std::endl;
		map_type copy(map);
		display_map(copy.begin(), copy.end());
		check_map(copy.begin(), copy.end());
	}

	std::cout << "[+] default with Compare and Allocator" << std::endl;
	{
		std::cout << "Original" << std::endl;
		display_map(map.begin(), map.end());

		std::cout << "Cmp and Alloc" << std::endl;
		map_type dfl_cmp_alloc = map_type(std::less<std::string>(), std::allocator<map_type::value_type>());
		dfl_cmp_alloc.insert(dfl_cmp_alloc.begin(), dfl_cmp_alloc.end());
		display_map(dfl_cmp_alloc.begin(), dfl_cmp_alloc.end());
		check_map(dfl_cmp_alloc.begin(), dfl_cmp_alloc.end());
	}

	std::cout << "[+] iterator constructor" << std::endl;
	{
		std::cout << "Original" << std::endl;
		display_map(map.begin(), map.end());

		std::cout << "Iterator" << std::endl;
		map_type copy(map.begin(), --(map.end()));
		display_map(copy.begin(), copy.end());
		check_map(copy.begin(), copy.end());
	}
}

template<typename _Iterator>
void test_one_iterator_trait() {
	std::cout << "sizeof iterator_traits::difference_type: "	<< sizeof(typename TEST_NAMESPACE::iterator_traits<_Iterator>::difference_type) << std::endl;
	std::cout << "sizeof iterator_traits::value_type: "			<< sizeof(typename TEST_NAMESPACE::iterator_traits<_Iterator>::value_type) << std::endl;
	std::cout << "sizeof iterator_traits::pointer: "			<< sizeof(typename TEST_NAMESPACE::iterator_traits<_Iterator>::pointer) << std::endl;
	std::cout << "sizeof iterator_traits::reference: "			<< sizeof(typename TEST_NAMESPACE::iterator_traits<_Iterator>::reference) << std::endl;
	std::cout << "sizeof iterator_traits::iterator_category: "	<< sizeof(typename TEST_NAMESPACE::iterator_traits<_Iterator>::iterator_category) << std::endl;
}

void test_iterator_traits() {
	std::cout << "======= iterator_traits TESTS =======" << std::endl;

	std::cout << "** ft::vector<int>::iterator **" << std::endl;
	test_one_iterator_trait< ft::vector<int>::iterator >();

	std::cout << "** ft::vector<int>::const_iterator **" << std::endl;
	test_one_iterator_trait< ft::vector<int>::const_iterator >();

	std::cout << "** ft::vector<int>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::vector<int>::reverse_iterator >();

	std::cout << "** ft::vector<int>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::vector<int>::const_reverse_iterator >();

	std::cout << "** ft::vector<std::string>::iterator **" << std::endl;
	test_one_iterator_trait< ft::vector<std::string>::iterator >();

	std::cout << "** ft::vector<std::string>::const_iterator **" << std::endl;
	test_one_iterator_trait< ft::vector<std::string>::const_iterator >();

	std::cout << "** ft::vector<std::string>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::vector<std::string>::reverse_iterator >();

	std::cout << "** ft::vector<std::string>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::vector<std::string>::const_reverse_iterator >();

	std::cout << "** ft::map<int, int>::iterator **" << std::endl;
	test_one_iterator_trait< ft::map<int, int>::iterator >();

	std::cout << "** ft::map<int, int>::const_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<int, int>::const_iterator >();

	std::cout << "** ft::map<int, int>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<int, int>::reverse_iterator >();

	std::cout << "** ft::map<int, int>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<int, int>::const_reverse_iterator >();

	std::cout << "** ft::map<int, int>::iterator **" << std::endl;
	test_one_iterator_trait< ft::map<int, int>::iterator >();

	std::cout << "** ft::map<int, int>::const_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<int, int>::const_iterator >();

	std::cout << "** ft::map<int, int>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<int, int>::reverse_iterator >();

	std::cout << "** ft::map<int, int>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<int, int>::const_reverse_iterator >();

	std::cout << "** ft::map<std::string, std::string>::iterator **" << std::endl;
	test_one_iterator_trait< ft::map<std::string, std::string>::iterator >();

	std::cout << "** ft::map<std::string, std::string>::const_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<std::string, std::string>::const_iterator >();

	std::cout << "** ft::map<std::string, std::string>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<std::string, std::string>::reverse_iterator >();

	std::cout << "** ft::map<std::string, std::string>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<std::string, std::string>::const_reverse_iterator >();

	std::cout << "** ft::map<std::string, std::string>::iterator **" << std::endl;
	test_one_iterator_trait< ft::map<std::string, std::string>::iterator >();

	std::cout << "** ft::map<std::string, std::string>::const_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<std::string, std::string>::const_iterator >();

	std::cout << "** ft::map<std::string, std::string>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<std::string, std::string>::reverse_iterator >();

	std::cout << "** ft::map<std::string, std::string>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< ft::map<std::string, std::string>::const_reverse_iterator >();


	std::cout << "** std::vector<int>::iterator **" << std::endl;
	test_one_iterator_trait< std::vector<int>::iterator >();

	std::cout << "** std::vector<int>::const_iterator **" << std::endl;
	test_one_iterator_trait< std::vector<int>::const_iterator >();

	std::cout << "** std::vector<int>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::vector<int>::reverse_iterator >();

	std::cout << "** std::vector<int>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::vector<int>::const_reverse_iterator >();

	std::cout << "** std::vector<std::string>::iterator **" << std::endl;
	test_one_iterator_trait< std::vector<std::string>::iterator >();

	std::cout << "** std::vector<std::string>::const_iterator **" << std::endl;
	test_one_iterator_trait< std::vector<std::string>::const_iterator >();

	std::cout << "** std::vector<std::string>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::vector<std::string>::reverse_iterator >();

	std::cout << "** std::vector<std::string>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::vector<std::string>::const_reverse_iterator >();

	std::cout << "** std::map<int, int>::iterator **" << std::endl;
	test_one_iterator_trait< std::map<int, int>::iterator >();

	std::cout << "** std::map<int, int>::const_iterator **" << std::endl;
	test_one_iterator_trait< std::map<int, int>::const_iterator >();

	std::cout << "** std::map<int, int>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::map<int, int>::reverse_iterator >();

	std::cout << "** std::map<int, int>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::map<int, int>::const_reverse_iterator >();

	std::cout << "** std::map<int, int>::iterator **" << std::endl;
	test_one_iterator_trait< std::map<int, int>::iterator >();

	std::cout << "** std::map<int, int>::const_iterator **" << std::endl;
	test_one_iterator_trait< std::map<int, int>::const_iterator >();

	std::cout << "** std::map<int, int>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::map<int, int>::reverse_iterator >();

	std::cout << "** std::map<int, int>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::map<int, int>::const_reverse_iterator >();

	std::cout << "** std::map<std::string, std::string>::iterator **" << std::endl;
	test_one_iterator_trait< std::map<std::string, std::string>::iterator >();

	std::cout << "** std::map<std::string, std::string>::const_iterator **" << std::endl;
	test_one_iterator_trait< std::map<std::string, std::string>::const_iterator >();

	std::cout << "** std::map<std::string, std::string>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::map<std::string, std::string>::reverse_iterator >();

	std::cout << "** std::map<std::string, std::string>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::map<std::string, std::string>::const_reverse_iterator >();

	std::cout << "** std::map<std::string, std::string>::iterator **" << std::endl;
	test_one_iterator_trait< std::map<std::string, std::string>::iterator >();

	std::cout << "** std::map<std::string, std::string>::const_iterator **" << std::endl;
	test_one_iterator_trait< std::map<std::string, std::string>::const_iterator >();

	std::cout << "** std::map<std::string, std::string>::reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::map<std::string, std::string>::reverse_iterator >();

	std::cout << "** std::map<std::string, std::string>::const_reverse_iterator **" << std::endl;
	test_one_iterator_trait< std::map<std::string, std::string>::const_reverse_iterator >();

	std::cout << "** int * **" << std::endl;
	test_one_iterator_trait<int *>();

	std::cout << "** const int * **" << std::endl;
	test_one_iterator_trait<const int *>();

	std::cout << "** std::string * **" << std::endl;
	test_one_iterator_trait<std::string *>();

	std::cout << "** const std::string * **" << std::endl;
	test_one_iterator_trait<const std::string *>();
}


template<typename _Tp>
void ft_enable_if(typename ft::enable_if< std::is_integral<_Tp>::value >::type* /* Yeah, weird-ass SFINAE from C++98 */) {
	std::cout << "It IS integral" << std::endl;
}

template<typename _Tp>
void ft_enable_if(...) {
	std::cout << "It is NOT integral" << std::endl;
}

template<typename _Tp>
void std_enable_if(typename std::enable_if< std::is_integral<_Tp>::value >::type* /* .............. */) {
	std::cout << "It IS integral" << std::endl;
}

template<typename _Tp>
void std_enable_if(...) {
	std::cout << "It is NOT integral" << std::endl;
}

void test_enable_if() {
	std::cout << "========== ENABLE_IF TESTS ==========" << std::endl;
	std::cout << "[+] int: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)<int>(NULL);
	std::cout << "[+] bool: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)<bool>(NULL);
	std::cout << "[+] std::string: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)<std::string>(NULL);
	std::cout << "[+] ft::vector<int>: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)< ft::vector<int> >(NULL);
	std::cout << "[+] ft::vector<std::string>: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)< ft::vector<std::string> >(NULL);
	std::cout << "[+] float: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)<float>(NULL);
	std::cout << "[+] double: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)<double>(NULL);
	std::cout << "[+] short: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)<short>(NULL);
	std::cout << "[+] long long int: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)<long long int>(NULL);
	std::cout << "[+] unsigned long long int: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)<unsigned long long int>(NULL);
	std::cout << "[+] wchar_t: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)<wchar_t>(NULL);
	std::cout << "[+] char32_t: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)<char32_t>(NULL);
	std::cout << "[+] std::map<int, int>: "; MAKE_NAME(TEST_NAMESPACE, _enable_if)< std::map<int, int> >(NULL);
}

void test_is_integral() {
	std::cout << "========== IS_INTEGRAL TESTS ==========" << std::endl;
	std::cout << "[+] bool: " << TEST_NAMESPACE::is_integral<bool>::value << std::endl;
	std::cout << "[+] char: " << TEST_NAMESPACE::is_integral<char>::value << std::endl;
	std::cout << "[+] wchar_t: " << TEST_NAMESPACE::is_integral<wchar_t>::value << std::endl;
	std::cout << "[+] short: " << TEST_NAMESPACE::is_integral<short>::value << std::endl;
	std::cout << "[+] int: " << TEST_NAMESPACE::is_integral<int>::value << std::endl;
	std::cout << "[+] long: " << TEST_NAMESPACE::is_integral<long>::value << std::endl;
	std::cout << "[+] long long: " << TEST_NAMESPACE::is_integral<long long>::value << std::endl;
	std::cout << "[+] unsigned char: " << TEST_NAMESPACE::is_integral<unsigned char>::value << std::endl;
	std::cout << "[+] unsigned short: " << TEST_NAMESPACE::is_integral<unsigned short>::value << std::endl;
	std::cout << "[+] unsigned int: " << TEST_NAMESPACE::is_integral<unsigned int>::value << std::endl;
	std::cout << "[+] unsigned long: " << TEST_NAMESPACE::is_integral<unsigned long>::value << std::endl;
	std::cout << "[+] unsigned long long: " << TEST_NAMESPACE::is_integral<unsigned long long>::value << std::endl;
	std::cout << "[+] signed char: " << TEST_NAMESPACE::is_integral<signed char>::value << std::endl;
	std::cout << "[+] signed short: " << TEST_NAMESPACE::is_integral<signed short>::value << std::endl;
	std::cout << "[+] signed int: " << TEST_NAMESPACE::is_integral<signed int>::value << std::endl;
	std::cout << "[+] signed long: " << TEST_NAMESPACE::is_integral<signed long>::value << std::endl;
	std::cout << "[+] signed long long: " << TEST_NAMESPACE::is_integral<signed long long>::value << std::endl;
	std::cout << "[+] const bool: " << TEST_NAMESPACE::is_integral<const bool>::value << std::endl;
	std::cout << "[+] const char: " << TEST_NAMESPACE::is_integral<const char>::value << std::endl;
	std::cout << "[+] const wchar_t: " << TEST_NAMESPACE::is_integral<const wchar_t>::value << std::endl;
	std::cout << "[+] const short: " << TEST_NAMESPACE::is_integral<const short>::value << std::endl;
	std::cout << "[+] const int: " << TEST_NAMESPACE::is_integral<const int>::value << std::endl;
	std::cout << "[+] const long: " << TEST_NAMESPACE::is_integral<const long>::value << std::endl;
	std::cout << "[+] const long long: " << TEST_NAMESPACE::is_integral<const long long>::value << std::endl;
	std::cout << "[+] const unsigned char: " << TEST_NAMESPACE::is_integral<const unsigned char>::value << std::endl;
	std::cout << "[+] const unsigned short: " << TEST_NAMESPACE::is_integral<const unsigned short>::value << std::endl;
	std::cout << "[+] const unsigned int: " << TEST_NAMESPACE::is_integral<const unsigned int>::value << std::endl;
	std::cout << "[+] const unsigned long: " << TEST_NAMESPACE::is_integral<const unsigned long>::value << std::endl;
	std::cout << "[+] const unsigned long long: " << TEST_NAMESPACE::is_integral<const unsigned long long>::value << std::endl;
	std::cout << "[+] const signed char: " << TEST_NAMESPACE::is_integral<const signed char>::value << std::endl;
	std::cout << "[+] const signed short: " << TEST_NAMESPACE::is_integral<const signed short>::value << std::endl;
	std::cout << "[+] const signed int: " << TEST_NAMESPACE::is_integral<const signed int>::value << std::endl;
	std::cout << "[+] const signed long: " << TEST_NAMESPACE::is_integral<const signed long>::value << std::endl;
	std::cout << "[+] const signed long long: " << TEST_NAMESPACE::is_integral<const signed long long>::value << std::endl;

	std::cout << "[+] std::string: " << TEST_NAMESPACE::is_integral<std::string>::value << std::endl;
	std::cout << "[+] std::string: " << TEST_NAMESPACE::is_integral<std::ofstream>::value << std::endl;
}

template<typename _Iterator0, typename _Iterator1>
void test_lexico_it(_Iterator0 begin0, _Iterator0 end0, _Iterator1 begin1, _Iterator1 end1) {
	std::cout << "[+] lexico (begin0, end0, begin1, end1): " << TEST_NAMESPACE::lexicographical_compare(begin0, end0, begin1, end1) << std::endl;
	std::cout << "[+] lexico (begin1, end1, begin0, end0): " << TEST_NAMESPACE::lexicographical_compare(begin1, end1, begin0, end0) << std::endl;
	std::cout << "[+] lexico (begin0, end0, begin0, end0): " << TEST_NAMESPACE::lexicographical_compare(begin0, end0, begin0, end0) << std::endl;
	std::cout << "[+] lexico (begin1, end1, begin1, end1): " << TEST_NAMESPACE::lexicographical_compare(begin1, end1, begin1, end1) << std::endl;
}

template<typename _Iterator0, typename _Iterator1>
void test_equal_lexico_it(_Iterator0 begin0, _Iterator0 end0, _Iterator1 begin1, _Iterator1 end1) {
	std::cout << "[+] equal  (begin0, end0, begin1): " << TEST_NAMESPACE::equal(begin0, end0, begin1) << std::endl;
	std::cout << "[+] equal  (begin1, end1, begin0): " << TEST_NAMESPACE::equal(begin1, end1, begin0) << std::endl;
	std::cout << "[+] equal  (begin0, end0, begin0): " << TEST_NAMESPACE::equal(begin0, end0, begin0) << std::endl;
	std::cout << "[+] equal  (begin1, end1, begin1): " << TEST_NAMESPACE::equal(begin1, end1, begin1) << std::endl;

	test_lexico_it(begin0, end0, begin1, end1);
}

void test_equal_lexico() {
	std::cout << "========== EQUAL/LEXICOGRAPHICAL_COMPARE TESTS ==========" << std::endl;

	std::vector<int> std_v_int_0;
	std::vector<int> std_v_int_1;
	std::vector<int> std_v_int_2;
	std::vector<int> std_v_int_3;
	std::vector<int> std_v_int_4;
	std::vector<int> std_v_int_5;
	std::vector<int> std_v_int_6;

	for (int i = 0; i < 30; ++i) {
		std_v_int_0.push_back(i);
		std_v_int_1.push_back(i);
		std_v_int_2.push_back((i * 3) ^ 42);

		if (i < 20) {
			std_v_int_3.push_back(i);
			std_v_int_4.push_back((i * 3) ^ 42);
		}
	}

	std::cout << "** same content, same vector **" << std::endl;
	test_equal_lexico_it(std_v_int_0.begin(), std_v_int_0.end(), std_v_int_0.begin(), std_v_int_0.end());

	std::cout << "** same content, different vector **" << std::endl;
	test_equal_lexico_it(std_v_int_0.begin(), std_v_int_0.end(), std_v_int_1.begin(), std_v_int_1.end());

	std::cout << "** different content, same size, different vector **" << std::endl;
	test_equal_lexico_it(std_v_int_0.begin(), std_v_int_0.end(), std_v_int_2.begin(), std_v_int_2.end());

	std::cout << "** different content, same size, different vector **" << std::endl;
	test_equal_lexico_it(std_v_int_2.begin(), std_v_int_2.end(), std_v_int_1.begin(), std_v_int_1.end());

	std::cout << "** different content, same size, different vector **" << std::endl;
	test_equal_lexico_it(std_v_int_0.begin(), std_v_int_0.end(), std_v_int_2.begin(), std_v_int_2.end());

	std::cout << "** same content, different size, different vector **" << std::endl;
	test_lexico_it(std_v_int_0.begin(), std_v_int_0.end(), std_v_int_3.begin(), std_v_int_3.end());

	std::cout << "** different content, different size, different vector **" << std::endl;
	test_lexico_it(std_v_int_0.begin(), std_v_int_0.end(), std_v_int_4.begin(), std_v_int_4.end());

	std::cout << "** one empty, different vector **" << std::endl;
	test_lexico_it(std_v_int_0.begin(), std_v_int_0.end(), std_v_int_5.begin(), std_v_int_5.end());

	std::cout << "** both empty, same vector **" << std::endl;
	test_equal_lexico_it(std_v_int_6.begin(), std_v_int_6.end(), std_v_int_5.begin(), std_v_int_5.end());

	std::cout << "** both empty, different vector **" << std::endl;
	test_equal_lexico_it(std_v_int_5.begin(), std_v_int_5.end(), std_v_int_5.begin(), std_v_int_5.end());
}

void test_pair() {
	std::cout << "========== EQUAL/LEXICOGRAPHICAL_COMPARE TESTS ==========" << std::endl;
	TEST_NAMESPACE::pair<int, int> p0;
	TEST_NAMESPACE::pair<int, int> p1(-69, 420);
	TEST_NAMESPACE::pair<short, short> p_tmp(1, 2);
	TEST_NAMESPACE::pair<short, int> p2(p_tmp);
	TEST_NAMESPACE::pair<int, int> p3(p_tmp);
	TEST_NAMESPACE::pair<int, int> p4(p1);
	TEST_NAMESPACE::pair<int, int> p5(0, 0);
	TEST_NAMESPACE::pair<short, int> p6(0, 0);
	TEST_NAMESPACE::pair<int, int> p7(0, 0);
	p5 = p2;
	p6 = p_tmp;
	p7 = p_tmp;

	std::cout << "[+] default initialized: " << p0 << " (!! differences expected !!)" << std::endl;
	std::cout << "[+] values initialized: " << p1 << std::endl;
	std::cout << "[+] copy (same _T0, different _T1): " << p2 << std::endl;
	std::cout << "[+] copy (different but constructible _T0 and _T1): " << p3 << std::endl;
	std::cout << "[+] copy (same _T0 and _T1): " << p4 << std::endl;
	std::cout << "[+] assigned (same _T0 and _T1): " << p5 << std::endl;
	std::cout << "[+] assigned (same _T0 and different _T1): " << p6 << std::endl;
	std::cout << "[+] assigned (different _T0 and _T1): " << p7 << std::endl;
	std::cout << "[+] make_pair (same _T0 and _T1): " << TEST_NAMESPACE::make_pair(50, 69);
	std::cout << "[+] make_pair (different _T0 and _T1): " << TEST_NAMESPACE::make_pair("BONCHOUR", 69);

	std::cout << "** == operator **" << std::endl;
	std::cout << "[+] p0 == p0: " << (p0 == p0) << std::endl;
	// std::cout << "[+] p0 == p1: " << (p0 == p1) << std::endl;
	// std::cout << "[+] p0 == p3: " << (p0 == p3) << std::endl;
	// std::cout << "[+] p0 == p4: " << (p0 == p4) << std::endl;
	// std::cout << "[+] p0 == p5: " << (p0 == p5) << std::endl;
	// std::cout << "[+] p0 == p7: " << (p0 == p7) << std::endl;
	// std::cout << "[+] p1 == p0: " << (p1 == p0) << std::endl;
	std::cout << "[+] p1 == p1: " << (p1 == p1) << std::endl;
	std::cout << "[+] p1 == p3: " << (p1 == p3) << std::endl;
	std::cout << "[+] p1 == p4: " << (p1 == p4) << std::endl;
	std::cout << "[+] p1 == p5: " << (p1 == p5) << std::endl;
	std::cout << "[+] p1 == p7: " << (p1 == p7) << std::endl;
	// std::cout << "[+] p3 == p0: " << (p3 == p0) << std::endl;
	std::cout << "[+] p3 == p1: " << (p3 == p1) << std::endl;
	std::cout << "[+] p3 == p3: " << (p3 == p3) << std::endl;
	std::cout << "[+] p3 == p4: " << (p3 == p4) << std::endl;
	std::cout << "[+] p3 == p5: " << (p3 == p5) << std::endl;
	std::cout << "[+] p3 == p7: " << (p3 == p7) << std::endl;
	// std::cout << "[+] p4 == p0: " << (p4 == p0) << std::endl;
	std::cout << "[+] p4 == p1: " << (p4 == p1) << std::endl;
	std::cout << "[+] p4 == p3: " << (p4 == p3) << std::endl;
	std::cout << "[+] p4 == p4: " << (p4 == p4) << std::endl;
	std::cout << "[+] p4 == p5: " << (p4 == p5) << std::endl;
	std::cout << "[+] p4 == p7: " << (p4 == p7) << std::endl;
	// std::cout << "[+] p5 == p0: " << (p5 == p0) << std::endl;
	std::cout << "[+] p5 == p1: " << (p5 == p1) << std::endl;
	std::cout << "[+] p5 == p3: " << (p5 == p3) << std::endl;
	std::cout << "[+] p5 == p4: " << (p5 == p4) << std::endl;
	std::cout << "[+] p5 == p5: " << (p5 == p5) << std::endl;
	std::cout << "[+] p5 == p7: " << (p5 == p7) << std::endl;
	// std::cout << "[+] p7 == p0: " << (p7 == p0) << std::endl;
	std::cout << "[+] p7 == p1: " << (p7 == p1) << std::endl;
	std::cout << "[+] p7 == p3: " << (p7 == p3) << std::endl;
	std::cout << "[+] p7 == p4: " << (p7 == p4) << std::endl;
	std::cout << "[+] p7 == p5: " << (p7 == p5) << std::endl;
	std::cout << "[+] p7 == p7: " << (p7 == p7) << std::endl;

	std::cout << "** != operator **" << std::endl;
	std::cout << "[+] p0 != p0: " << (p0 != p0) << std::endl;
	// std::cout << "[+] p0 != p1: " << (p0 != p1) << std::endl;
	// std::cout << "[+] p0 != p3: " << (p0 != p3) << std::endl;
	// std::cout << "[+] p0 != p4: " << (p0 != p4) << std::endl;
	// std::cout << "[+] p0 != p5: " << (p0 != p5) << std::endl;
	// std::cout << "[+] p0 != p7: " << (p0 != p7) << std::endl;
	// std::cout << "[+] p1 != p0: " << (p1 != p0) << std::endl;
	std::cout << "[+] p1 != p1: " << (p1 != p1) << std::endl;
	std::cout << "[+] p1 != p3: " << (p1 != p3) << std::endl;
	std::cout << "[+] p1 != p4: " << (p1 != p4) << std::endl;
	std::cout << "[+] p1 != p5: " << (p1 != p5) << std::endl;
	std::cout << "[+] p1 != p7: " << (p1 != p7) << std::endl;
	// std::cout << "[+] p3 != p0: " << (p3 != p0) << std::endl;
	std::cout << "[+] p3 != p1: " << (p3 != p1) << std::endl;
	std::cout << "[+] p3 != p3: " << (p3 != p3) << std::endl;
	std::cout << "[+] p3 != p4: " << (p3 != p4) << std::endl;
	std::cout << "[+] p3 != p5: " << (p3 != p5) << std::endl;
	std::cout << "[+] p3 != p7: " << (p3 != p7) << std::endl;
	// std::cout << "[+] p4 != p0: " << (p4 != p0) << std::endl;
	std::cout << "[+] p4 != p1: " << (p4 != p1) << std::endl;
	std::cout << "[+] p4 != p3: " << (p4 != p3) << std::endl;
	std::cout << "[+] p4 != p4: " << (p4 != p4) << std::endl;
	std::cout << "[+] p4 != p5: " << (p4 != p5) << std::endl;
	std::cout << "[+] p4 != p7: " << (p4 != p7) << std::endl;
	// std::cout << "[+] p5 != p0: " << (p5 != p0) << std::endl;
	std::cout << "[+] p5 != p1: " << (p5 != p1) << std::endl;
	std::cout << "[+] p5 != p3: " << (p5 != p3) << std::endl;
	std::cout << "[+] p5 != p4: " << (p5 != p4) << std::endl;
	std::cout << "[+] p5 != p5: " << (p5 != p5) << std::endl;
	std::cout << "[+] p5 != p7: " << (p5 != p7) << std::endl;
	// std::cout << "[+] p7 != p0: " << (p7 != p0) << std::endl;
	std::cout << "[+] p7 != p1: " << (p7 != p1) << std::endl;
	std::cout << "[+] p7 != p3: " << (p7 != p3) << std::endl;
	std::cout << "[+] p7 != p4: " << (p7 != p4) << std::endl;
	std::cout << "[+] p7 != p5: " << (p7 != p5) << std::endl;
	std::cout << "[+] p7 != p7: " << (p7 != p7) << std::endl;

	std::cout << "** < operator **" << std::endl;
	std::cout << "[+] p0 < p0: " << (p0 < p0) << std::endl;
	// std::cout << "[+] p0 < p1: " << (p0 < p1) << std::endl;
	// std::cout << "[+] p0 < p3: " << (p0 < p3) << std::endl;
	// std::cout << "[+] p0 < p4: " << (p0 < p4) << std::endl;
	// std::cout << "[+] p0 < p5: " << (p0 < p5) << std::endl;
	// std::cout << "[+] p0 < p7: " << (p0 < p7) << std::endl;
	// std::cout << "[+] p1 < p0: " << (p1 < p0) << std::endl;
	std::cout << "[+] p1 < p1: " << (p1 < p1) << std::endl;
	std::cout << "[+] p1 < p3: " << (p1 < p3) << std::endl;
	std::cout << "[+] p1 < p4: " << (p1 < p4) << std::endl;
	std::cout << "[+] p1 < p5: " << (p1 < p5) << std::endl;
	std::cout << "[+] p1 < p7: " << (p1 < p7) << std::endl;
	// std::cout << "[+] p3 < p0: " << (p3 < p0) << std::endl;
	std::cout << "[+] p3 < p1: " << (p3 < p1) << std::endl;
	std::cout << "[+] p3 < p3: " << (p3 < p3) << std::endl;
	std::cout << "[+] p3 < p4: " << (p3 < p4) << std::endl;
	std::cout << "[+] p3 < p5: " << (p3 < p5) << std::endl;
	std::cout << "[+] p3 < p7: " << (p3 < p7) << std::endl;
	// std::cout << "[+] p4 < p0: " << (p4 < p0) << std::endl;
	std::cout << "[+] p4 < p1: " << (p4 < p1) << std::endl;
	std::cout << "[+] p4 < p3: " << (p4 < p3) << std::endl;
	std::cout << "[+] p4 < p4: " << (p4 < p4) << std::endl;
	std::cout << "[+] p4 < p5: " << (p4 < p5) << std::endl;
	std::cout << "[+] p4 < p7: " << (p4 < p7) << std::endl;
	// std::cout << "[+] p5 < p0: " << (p5 < p0) << std::endl;
	std::cout << "[+] p5 < p1: " << (p5 < p1) << std::endl;
	std::cout << "[+] p5 < p3: " << (p5 < p3) << std::endl;
	std::cout << "[+] p5 < p4: " << (p5 < p4) << std::endl;
	std::cout << "[+] p5 < p5: " << (p5 < p5) << std::endl;
	std::cout << "[+] p5 < p7: " << (p5 < p7) << std::endl;
	std::cout << "[+] p7 < p0: " << (p7 < p0) << std::endl;
	std::cout << "[+] p7 < p1: " << (p7 < p1) << std::endl;
	std::cout << "[+] p7 < p3: " << (p7 < p3) << std::endl;
	std::cout << "[+] p7 < p4: " << (p7 < p4) << std::endl;
	std::cout << "[+] p7 < p5: " << (p7 < p5) << std::endl;
	std::cout << "[+] p7 < p7: " << (p7 < p7) << std::endl;

	std::cout << "** <= operator **" << std::endl;
	std::cout << "[+] p0 <= p0: " << (p0 <= p0) << std::endl;
	// std::cout << "[+] p0 <= p1: " << (p0 <= p1) << std::endl;
	// std::cout << "[+] p0 <= p3: " << (p0 <= p3) << std::endl;
	// std::cout << "[+] p0 <= p4: " << (p0 <= p4) << std::endl;
	// std::cout << "[+] p0 <= p5: " << (p0 <= p5) << std::endl;
	// std::cout << "[+] p0 <= p7: " << (p0 <= p7) << std::endl;
	// std::cout << "[+] p1 <= p0: " << (p1 <= p0) << std::endl;
	std::cout << "[+] p1 <= p1: " << (p1 <= p1) << std::endl;
	std::cout << "[+] p1 <= p3: " << (p1 <= p3) << std::endl;
	std::cout << "[+] p1 <= p4: " << (p1 <= p4) << std::endl;
	std::cout << "[+] p1 <= p5: " << (p1 <= p5) << std::endl;
	std::cout << "[+] p1 <= p7: " << (p1 <= p7) << std::endl;
	// std::cout << "[+] p3 <= p0: " << (p3 <= p0) << std::endl;
	std::cout << "[+] p3 <= p1: " << (p3 <= p1) << std::endl;
	std::cout << "[+] p3 <= p3: " << (p3 <= p3) << std::endl;
	std::cout << "[+] p3 <= p4: " << (p3 <= p4) << std::endl;
	std::cout << "[+] p3 <= p5: " << (p3 <= p5) << std::endl;
	std::cout << "[+] p3 <= p7: " << (p3 <= p7) << std::endl;
	// std::cout << "[+] p4 <= p0: " << (p4 <= p0) << std::endl;
	std::cout << "[+] p4 <= p1: " << (p4 <= p1) << std::endl;
	std::cout << "[+] p4 <= p3: " << (p4 <= p3) << std::endl;
	std::cout << "[+] p4 <= p4: " << (p4 <= p4) << std::endl;
	std::cout << "[+] p4 <= p5: " << (p4 <= p5) << std::endl;
	std::cout << "[+] p4 <= p7: " << (p4 <= p7) << std::endl;
	// std::cout << "[+] p5 <= p0: " << (p5 <= p0) << std::endl;
	std::cout << "[+] p5 <= p1: " << (p5 <= p1) << std::endl;
	std::cout << "[+] p5 <= p3: " << (p5 <= p3) << std::endl;
	std::cout << "[+] p5 <= p4: " << (p5 <= p4) << std::endl;
	std::cout << "[+] p5 <= p5: " << (p5 <= p5) << std::endl;
	std::cout << "[+] p5 <= p7: " << (p5 <= p7) << std::endl;
	// std::cout << "[+] p7 <= p0: " << (p7 <= p0) << std::endl;
	std::cout << "[+] p7 <= p1: " << (p7 <= p1) << std::endl;
	std::cout << "[+] p7 <= p3: " << (p7 <= p3) << std::endl;
	std::cout << "[+] p7 <= p4: " << (p7 <= p4) << std::endl;
	std::cout << "[+] p7 <= p5: " << (p7 <= p5) << std::endl;
	std::cout << "[+] p7 <= p7: " << (p7 <= p7) << std::endl;

	std::cout << "** > operator **" << std::endl;
	std::cout << "[+] p0 > p0: " << (p0 > p0) << std::endl;
	// std::cout << "[+] p0 > p1: " << (p0 > p1) << std::endl;
	// std::cout << "[+] p0 > p3: " << (p0 > p3) << std::endl;
	// std::cout << "[+] p0 > p4: " << (p0 > p4) << std::endl;
	// std::cout << "[+] p0 > p5: " << (p0 > p5) << std::endl;
	// std::cout << "[+] p0 > p7: " << (p0 > p7) << std::endl;
	// std::cout << "[+] p1 > p0: " << (p1 > p0) << std::endl;
	std::cout << "[+] p1 > p1: " << (p1 > p1) << std::endl;
	std::cout << "[+] p1 > p3: " << (p1 > p3) << std::endl;
	std::cout << "[+] p1 > p4: " << (p1 > p4) << std::endl;
	std::cout << "[+] p1 > p5: " << (p1 > p5) << std::endl;
	std::cout << "[+] p1 > p7: " << (p1 > p7) << std::endl;
	// std::cout << "[+] p3 > p0: " << (p3 > p0) << std::endl;
	std::cout << "[+] p3 > p1: " << (p3 > p1) << std::endl;
	std::cout << "[+] p3 > p3: " << (p3 > p3) << std::endl;
	std::cout << "[+] p3 > p4: " << (p3 > p4) << std::endl;
	std::cout << "[+] p3 > p5: " << (p3 > p5) << std::endl;
	std::cout << "[+] p3 > p7: " << (p3 > p7) << std::endl;
	// std::cout << "[+] p4 > p0: " << (p4 > p0) << std::endl;
	std::cout << "[+] p4 > p1: " << (p4 > p1) << std::endl;
	std::cout << "[+] p4 > p3: " << (p4 > p3) << std::endl;
	std::cout << "[+] p4 > p4: " << (p4 > p4) << std::endl;
	std::cout << "[+] p4 > p5: " << (p4 > p5) << std::endl;
	std::cout << "[+] p4 > p7: " << (p4 > p7) << std::endl;
	// std::cout << "[+] p5 > p0: " << (p5 > p0) << std::endl;
	std::cout << "[+] p5 > p1: " << (p5 > p1) << std::endl;
	std::cout << "[+] p5 > p3: " << (p5 > p3) << std::endl;
	std::cout << "[+] p5 > p4: " << (p5 > p4) << std::endl;
	std::cout << "[+] p5 > p5: " << (p5 > p5) << std::endl;
	std::cout << "[+] p5 > p7: " << (p5 > p7) << std::endl;
	// std::cout << "[+] p7 > p0: " << (p7 > p0) << std::endl;
	std::cout << "[+] p7 > p1: " << (p7 > p1) << std::endl;
	std::cout << "[+] p7 > p3: " << (p7 > p3) << std::endl;
	std::cout << "[+] p7 > p4: " << (p7 > p4) << std::endl;
	std::cout << "[+] p7 > p5: " << (p7 > p5) << std::endl;
	std::cout << "[+] p7 > p7: " << (p7 > p7) << std::endl;

	std::cout << "** >= operator **" << std::endl;
	std::cout << "[+] p0 >= p0: " << (p0 >= p0) << std::endl;
	// std::cout << "[+] p0 >= p1: " << (p0 >= p1) << std::endl;
	// std::cout << "[+] p0 >= p3: " << (p0 >= p3) << std::endl;
	// std::cout << "[+] p0 >= p4: " << (p0 >= p4) << std::endl;
	// std::cout << "[+] p0 >= p5: " << (p0 >= p5) << std::endl;
	// std::cout << "[+] p0 >= p7: " << (p0 >= p7) << std::endl;
	// std::cout << "[+] p1 >= p0: " << (p1 >= p0) << std::endl;
	std::cout << "[+] p1 >= p1: " << (p1 >= p1) << std::endl;
	std::cout << "[+] p1 >= p3: " << (p1 >= p3) << std::endl;
	std::cout << "[+] p1 >= p4: " << (p1 >= p4) << std::endl;
	std::cout << "[+] p1 >= p5: " << (p1 >= p5) << std::endl;
	std::cout << "[+] p1 >= p7: " << (p1 >= p7) << std::endl;
	// std::cout << "[+] p3 >= p0: " << (p3 >= p0) << std::endl;
	std::cout << "[+] p3 >= p1: " << (p3 >= p1) << std::endl;
	std::cout << "[+] p3 >= p3: " << (p3 >= p3) << std::endl;
	std::cout << "[+] p3 >= p4: " << (p3 >= p4) << std::endl;
	std::cout << "[+] p3 >= p5: " << (p3 >= p5) << std::endl;
	std::cout << "[+] p3 >= p7: " << (p3 >= p7) << std::endl;
	// std::cout << "[+] p4 >= p0: " << (p4 >= p0) << std::endl;
	std::cout << "[+] p4 >= p1: " << (p4 >= p1) << std::endl;
	std::cout << "[+] p4 >= p3: " << (p4 >= p3) << std::endl;
	std::cout << "[+] p4 >= p4: " << (p4 >= p4) << std::endl;
	std::cout << "[+] p4 >= p5: " << (p4 >= p5) << std::endl;
	std::cout << "[+] p4 >= p7: " << (p4 >= p7) << std::endl;
	// std::cout << "[+] p5 >= p0: " << (p5 >= p0) << std::endl;
	std::cout << "[+] p5 >= p1: " << (p5 >= p1) << std::endl;
	std::cout << "[+] p5 >= p3: " << (p5 >= p3) << std::endl;
	std::cout << "[+] p5 >= p4: " << (p5 >= p4) << std::endl;
	std::cout << "[+] p5 >= p5: " << (p5 >= p5) << std::endl;
	std::cout << "[+] p5 >= p7: " << (p5 >= p7) << std::endl;
	// std::cout << "[+] p7 >= p0: " << (p7 >= p0) << std::endl;
	std::cout << "[+] p7 >= p1: " << (p7 >= p1) << std::endl;
	std::cout << "[+] p7 >= p3: " << (p7 >= p3) << std::endl;
	std::cout << "[+] p7 >= p4: " << (p7 >= p4) << std::endl;
	std::cout << "[+] p7 >= p5: " << (p7 >= p5) << std::endl;
	std::cout << "[+] p7 >= p7: " << (p7 >= p7) << std::endl;

}

int main(void) {
	ULDL_INIT

	std::cout << "TEST_NAMESPACE=" << MACRO_TO_STRING(TEST_NAMESPACE) << std::endl;

	vector_test();

	TEST_NAMESPACE::stack<validity_sanitizer> s;
	s.push(validity_sanitizer(1));
	s.push(validity_sanitizer(2));
	s.pop();
	s.pop();

	test_tree();
	test_map();

	test_iterator_traits();

	test_enable_if();
	test_is_integral();

	test_equal_lexico();

	test_pair();

	return 0;
}
