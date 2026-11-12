#include "test_prelude.hpp"

namespace generic {
#include "lib/data_structure/sparse_table/sparse_table.cpp"
}
namespace min_table {
#include "lib/data_structure/sparse_table/min_sparse_table.cpp"
}
namespace max_table {
#include "lib/data_structure/sparse_table/max_sparse_table.cpp"
}

// Idempotent range queries over inclusive [l, r].
template <class Op>
ll brute(const vector<ll> &a, int l, int r, Op op) {
    ll res = a[l];
    for (int i = l + 1; i <= r; i++) res = op(res, a[i]);
    return res;
}

const auto kMin = [](ll x, ll y) { return min(x, y); };
const auto kMax = [](ll x, ll y) { return max(x, y); };
const auto kGcd = [](ll x, ll y) { return gcd(x, y); };

TEST(SparseTable, MinMaxGcdMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<ll> a = g.vec(n, -50, 50), pos = g.vec(n, 1, 60);
            generic::sparse_table mn(a, kMin);
            generic::sparse_table mx(a, kMax);
            generic::sparse_table gc(pos, kGcd);
            min_table::min_sparse_table<ll> mn2(a);
            max_table::max_sparse_table<ll> mx2(a);
            for (int q = 0; q < 100; q++) {
                auto [l, r] = g.segment(n);
                ASSERT_EQ(mn.query(l, r), brute(a, l, r, kMin)) << seed << " " << n;
                ASSERT_EQ(mx.query(l, r), brute(a, l, r, kMax)) << seed << " " << n;
                ASSERT_EQ(gc.query(l, r), brute(pos, l, r, kGcd)) << seed << " " << n;
                ASSERT_EQ(mn2.query(l, r), brute(a, l, r, kMin)) << seed << " " << n;
                ASSERT_EQ(mx2.query(l, r), brute(a, l, r, kMax)) << seed << " " << n;
            }
        }
    }
}

TEST(SparseTable, EdgeCases) {
    const vector<ll> one{-7};
    EXPECT_EQ(min_table::min_sparse_table<ll>(one).query(0, 0), -7);
    const vector<ll> a{INF, -INF, 0, 3};
    EXPECT_EQ(min_table::min_sparse_table<ll>(a).query(0, 3), -INF);
    EXPECT_EQ(max_table::max_sparse_table<ll>(a).query(1, 3), 3);
    EXPECT_EQ(generic::sparse_table(a, kMax).query(0, 0), INF);
}
