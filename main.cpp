#include "iterator.hpp"
#include "type_traits.hpp"
#include "algorithm.hpp"
#include "utility.hpp"

#include <vector>

int main(void) {
	return 0;
	std::vector<int> v;
	ft::reverse_iterator<std::vector<int>::iterator> test(v.end());
}
