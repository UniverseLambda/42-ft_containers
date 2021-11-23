#include "iterator.hpp"
#include "type_traits.hpp"
#include "algorithm.hpp"
#include "utility.hpp"

#include "Vector.hpp"

int main(void) {
	ft::__clsaad_impl::PtrIterator<int, ft::vector<int> > it;

	ft::iterator_traits<ft::__clsaad_impl::PtrIterator<int, ft::vector<int> > >::value_type i;

	i = 50;

	return i;
}
