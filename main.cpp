#include "iterator.hpp"

#include <vector>

int main(void) {
	return 0;
	std::vector<int> v;
	ft::reverse_iterator<std::vector<int>::iterator> test(v.end());
}
