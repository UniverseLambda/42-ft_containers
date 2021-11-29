#include "iterator.hpp"
#include "type_traits.hpp"
#include "algorithm.hpp"
#include "utility.hpp"

#include "Vector.hpp"

#include <ostream>
#include <iostream>

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

void checkVector(const ft::vector<ValiditySanitizer> &validity, size_t testIdx) {
	typedef ft::vector<ValiditySanitizer>::const_iterator iter;

	std::cout << "== CHECK " << testIdx << " ==" << std::endl;
	for (iter it = validity.begin(); it != validity.end(); ++it) {
		std::cout << "Checking " << it->idx << std::endl;
		it->check();
	}
}

void vectorTest() {
	ft::vector<ValiditySanitizer> validity;

	for (size_t i = 0; i < 5; ++i) {
		validity.push_back(ValiditySanitizer(i));
	}

	checkVector(validity, 0);
	validity.insert(validity.begin() + 5, ValiditySanitizer(666));
	checkVector(validity, 1);
	validity.insert(validity.begin() + 5, 6, ValiditySanitizer(667));
	checkVector(validity, 2);
	validity.insert(validity.begin() + 2, 42, ValiditySanitizer(69420));
	checkVector(validity, 3);
	validity.insert(validity.begin() + 2, 0, ValiditySanitizer(99999));
	checkVector(validity, 4);

}

int main(void) {
	vectorTest();

	return 0;
}
