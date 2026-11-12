#include "test_prelude.hpp"

namespace impl {
#include "lib/data_structure/fenwick/fenwick.cpp"
}

// Adapter: the behaviour under test, independent of the snippet's spelling.
struct Fenwick {
    impl::fenwick<ll> f;
    explicit Fenwick(int n) : f(n) {}
    void add(int i, ll x) { f.add(i, x); }
    ll sum(int l, int r) { return f.sum(l, r); }  // [l, r)
};

TEST(Fenwick, MatchesBruteForceOnRandomOps) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            Fenwick f(n);
            vector<ll> a(n);
            for (int op = 0; op < 200; op++) {
                if (g.coin()) {
                    int i = g.range(0, n - 1);
                    ll x = g.range(-1'000'000'000'000LL, 1'000'000'000'000LL);
                    f.add(i, x), a[i] += x;
                } else {
                    int l = g.range(0, n), r = g.range(0, n);
                    if (l > r) swap(l, r);
                    ASSERT_EQ(f.sum(l, r), accumulate(a.begin() + l, a.begin() + r, 0LL))
                        << "seed=" << seed << " n=" << n << " [" << l << "," << r << ")";
                    ASSERT_EQ(f.f.sum(r), accumulate(a.begin(), a.begin() + r, 0LL));  // prefix [0, r)
                }
            }
        }
    }
}

TEST(Fenwick, EdgeCases) {
    Fenwick f(1);
    EXPECT_EQ(f.sum(0, 0), 0);  // empty range
    f.add(0, -5);
    EXPECT_EQ(f.sum(0, 1), -5);
    EXPECT_EQ(f.sum(1, 1), 0);

    Fenwick big(8);
    for (int i = 0; i < 8; i++) big.add(i, INF / 16);  // no overflow up to ~INF/2
    EXPECT_EQ(big.sum(0, 8), INF / 16 * 8);
    EXPECT_EQ(big.sum(7, 8), INF / 16);
}
