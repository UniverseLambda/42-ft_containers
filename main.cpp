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
	return (os << " {idx: " << san.idx << ", origin: " << san.origin << ", valid: " << san.valid << "}");
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

	return 0;
}
