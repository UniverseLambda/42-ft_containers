#include "Vector.hpp"
#include <uldl/uldl.hpp>

struct TrackedWrapper {
	uldl::tracker<TrackedWrapper> tracker;
	char *leaker;

	TrackedWrapper(): tracker(this), leaker(new char[1]) {}
	TrackedWrapper(const TrackedWrapper &): tracker(this), leaker(new char[1]) {}
	~TrackedWrapper() { delete[] leaker; }

	TrackedWrapper &operator=(const TrackedWrapper &) { return *this; }
};

int main() {
	uldl::init();

	ft::vector<TrackedWrapper> v0;
	ft::vector<TrackedWrapper> v1;

	v0.assign(10, TrackedWrapper());
	v1.assign(2, TrackedWrapper());

	v0.insert(v0.begin() + 5, v1.begin(), v1.end());

	return 0;
}
