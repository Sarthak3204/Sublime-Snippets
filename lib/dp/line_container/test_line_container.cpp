#include "test_prelude.hpp"

namespace impl {
#include "lib/dp/line_container/line_container.cpp"
}

// Max of lines y = k*x + m at a point (convex hull trick).
TEST(LineContainer, MaxAtPointMatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : {1, 2, 3, 10, 50}) {
            impl::line_container<ll> h;
            vector<pair<ll, ll>> lines;
            for (int i = 0; i < n; i++) {
                ll k = g.range(-20, 20), m = g.range(-1000, 1000);  // duplicates & parallels likely
                h.insert(k, m);
                lines.push_back({k, m});
                for (int q = 0; q < 20; q++) {
                    ll x = g.range(-100, 100);
                    ll best = LLONG_MIN;
                    for (auto [kk, mm] : lines) best = max(best, kk * x + mm);
                    ASSERT_EQ(h.query(x), best) << seed << " x=" << x;
                }
            }
        }
    }
}

TEST(LineContainer, LargeCoordinates) {
    impl::line_container<ll> h;
    h.insert(1'000'000'000, -1'000'000'000'000'000'000LL);
    h.insert(-1'000'000'000, 0);
    EXPECT_EQ(h.query(1'000'000'000), 0);
    EXPECT_EQ(h.query(-1'000'000'000), 1'000'000'000'000'000'000LL);
}
