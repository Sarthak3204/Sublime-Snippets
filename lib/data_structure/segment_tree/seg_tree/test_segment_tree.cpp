#include "test_prelude.hpp"

namespace impl {
#include "lib/data_structure/segment_tree/seg_tree/seg_tree.cpp"
}

const auto kPlus = [](ll x, ll y) { return x + y; };

TEST(SegTree, PointAssignRangeSumMatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<ll> a = g.vec(n, -1'000'000'000, 1'000'000'000);
            impl::seg_tree t(a, 0LL, kPlus);
            for (int op = 0; op < 200; op++) {
                auto [l, r] = g.segment(n);
                if (g.coin()) {
                    if (g.coin()) r = l;  // mostly point updates, sometimes range assignment
                    ll v = g.range(-1'000'000'000, 1'000'000'000);
                    t.update(l, r, v);
                    for (int i = l; i <= r; i++) a[i] = v;
                } else {
                    ASSERT_EQ(t.query(l, r), accumulate(a.begin() + l, a.begin() + r + 1, 0LL))
                        << "seed=" << seed << " n=" << n << " [" << l << "," << r << "]";
                }
            }
        }
    }
}

// Non-commutative merge (affine composition) catches left/right order mistakes.
struct Affine {
    ll a, b;  // x -> a*x + b (mod)
    bool operator==(const Affine &) const = default;
};
constexpr ll M = 1'000'000'007;
const auto kCompose = [](Affine f, Affine g) { return Affine{f.a * g.a % M, (f.b * g.a + g.b) % M}; };  // f then g

TEST(SegTree, NonCommutativeMergeKeepsOrder) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<Affine> a(n);
            for (auto &f : a) f = {g.range(0, 9), g.range(0, 9)};
            impl::seg_tree t(a, Affine{1, 0}, kCompose);
            for (int op = 0; op < 100; op++) {
                if (g.coin()) {
                    int i = g.range(0, n - 1);
                    a[i] = {g.range(0, 9), g.range(0, 9)};
                    t.update(i, i, a[i]);
                }
                auto [l, r] = g.segment(n);
                Affine want{1, 0};
                for (int i = l; i <= r; i++) want = kCompose(want, a[i]);
                ASSERT_EQ(t.query(l, r), want) << seed << " n=" << n;
            }
        }
    }
}

TEST(SegTree, EdgeCases) {
    impl::seg_tree one(vector<ll>{7}, 0LL, kPlus);
    EXPECT_EQ(one.query(0, 0), 7);
    one.update(0, 0, -3);
    EXPECT_EQ(one.query(0, 0), -3);

    impl::seg_tree t(vector<ll>{1, 2, 3, 4, 5}, 0LL, kPlus);  // non power of two
    EXPECT_EQ(t.query(0, 4), 15);
    EXPECT_EQ(t.query(4, 4), 5);
    EXPECT_EQ(t.query(2, 1), 0);  // empty (l > r) -> identity
    t.update(0, 3, 10);           // range assignment (the original only handled l == r)
    EXPECT_EQ(t.query(0, 4), 45);

    impl::seg_tree sized(3, 0LL, kPlus);  // (n, identity) constructor
    EXPECT_EQ(sized.query(0, 2), 0);
}
