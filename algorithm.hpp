#pragma once

namespace ft {
	template<typename _It0, typename _It1>
	bool equal(_It0 it0, _It0 end0, _It1 it1) {
		for (; it0 != end0; ++it0, ++it1) {
			if (!(*it0 == *it1)) {
				return false;
			}
		}
		return true;
	}

	template<typename _It0, typename _It1, typename _BinaryPredicate>
	bool equal(_It0 it0, _It0 end0, _It1 it1, _BinaryPredicate predicate) {
		for (; it0 != end0; ++it0, ++it1) {
			if (!predicate(*it0, *it1)) {
				return false;
			}
		}
		return true;
	}

	template<typename _It0, typename _It1>
	bool lexicographical_compare(_It0 it0, _It0 end0, _It1 it1, _It1 end1) {
		for (; (it0 != end0) && (it1 != end1); ++it0, ++it1) {
			if (*it0 < *it1) {
				return true;
			} else if (*it1 < *it0) {
				return false;
			}
		}

		if (it0 == end0 && it1 != end1) {
			return true;
		}

		return false;
	}

	template<typename _It0, typename _It1, typename _Compare>
	bool lexicographical_compare(_It0 it0, _It0 end0, _It1 it1, _It1 end1, _Compare compare) {
		for (; (it0 != end0) && (it1 != end1); ++it0, ++it1) {
			if (compare(*it0, *it1)) {
				return true;
			} else if (compare(*it1, *it0)) {
				return false;
			}
		}

		if (it0 == end0 && it1 != end1) {
			return true;
		}

		return false;
	}
} // namespace ft
