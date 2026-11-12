#include "test_prelude.hpp"

namespace impl {
#include "lib/data_structure/mo/hilbert_order.cpp"
}  // namespace impl

TEST(Mo, HilbertOrderIsInjective) {
    set<ll> seen;  // distinct keys on a 64 x 64 grid
    for (int x = 0; x < 64; x++)
        for (int y = 0; y < 64; y++) ASSERT_TRUE(seen.insert(impl::hilbertorder(x, y)).second) << x << " " << y;
}
