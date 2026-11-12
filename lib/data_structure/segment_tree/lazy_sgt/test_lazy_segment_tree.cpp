#include "test_prelude.hpp"

namespace impl {
#include "lib/data_structure/segment_tree/lazy_sgt/lazy_sgt.cpp"
#include "lib/data_structure/segment_tree/lazy_sgt/generic_lazy_sgt.cpp"
}

TEST(LazySegTree, RangeAddRangeSumMatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<ll> a = g.vec(n, -1'000'000, 1'000'000);
            impl::lazy_sgt<ll> t(a);
            vector<impl::node_value> init;
            for (ll x : a) init.push_back({x, 1});
            impl::generic_lazy_sgt<impl::node_value, impl::node_update> gt(init);  // the example policy
            for (int op = 0; op < 200; op++) {
                auto [l, r] = g.segment(n);
                if (g.coin()) {
                    ll x = g.range(-1'000'000, 1'000'000);
                    t.update(l, r, x);
                    gt.update(l, r, {x});
                    for (int i = l; i <= r; i++) a[i] += x;
                } else {
                    ll want = accumulate(a.begin() + l, a.begin() + r + 1, 0LL);
                    ASSERT_EQ(t.query(l, r), want) << "seed=" << seed << " n=" << n << " [" << l << "," << r << "]";
                    ASSERT_EQ(gt.query(l, r).sum, want);
                    ASSERT_EQ(gt.query(l, r).len, r - l + 1);
                }
            }
        }
    }
}

// generic_lazy_sgt with a non-commutative update: range affine x -> b*x + c, range sum mod p.
constexpr ll P = 998244353;
struct affine_value {
    ll sum = 0, len = 0;
    affine_value operator+(const affine_value &o) const { return {(sum + o.sum) % P, len + o.len}; }
};
struct affine_update {
    ll b = 1, c = 0;  // identity
    void operator+=(const affine_update &o) { b = o.b * b % P, c = (o.b * c + o.c) % P; }  // this, then o
};
inline void operator+=(affine_value &v, const affine_update &u) { v.sum = (u.b * v.sum + u.c * v.len) % P; }

TEST(LazySegTree, GenericWithNonCommutativeUpdates) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<ll> a = g.vec(n, 0, 9);
            vector<affine_value> init;
            for (ll x : a) init.push_back({x, 1});
            impl::generic_lazy_sgt<affine_value, affine_update> t(init);
            for (int op = 0; op < 150; op++) {
                auto [l, r] = g.segment(n);
                if (g.coin()) {
                    ll b = g.range(0, 3), c = g.range(0, 3);
                    t.update(l, r, {b, c});
                    for (int i = l; i <= r; i++) a[i] = (b * a[i] + c) % P;
                } else {
                    ll want = 0;
                    for (int i = l; i <= r; i++) want = (want + a[i]) % P;
                    ASSERT_EQ(t.query(l, r).sum, want) << seed << " n=" << n;
                }
            }
        }
    }
}

TEST(LazySegTree, EdgeCases) {
    impl::lazy_sgt<ll> one(vector<ll>{5});
    one.update(0, 0, -5);
    EXPECT_EQ(one.query(0, 0), 0);

    impl::lazy_sgt<ll> t(vector<ll>(5, 0));
    t.update(0, 4, 3);  // full range
    t.update(1, 1, 1);  // single point inside a lazy range
    EXPECT_EQ(t.query(0, 4), 16);
    EXPECT_EQ(t.query(1, 1), 4);
    EXPECT_EQ(t.query(4, 4), 3);
    EXPECT_EQ(t.query(3, 2), 0);  // empty (l > r)

    impl::lazy_sgt<ll> sized(3);
    EXPECT_EQ(sized.query(0, 2), 0);
}
