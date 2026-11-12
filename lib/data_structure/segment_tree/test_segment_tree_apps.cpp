#include "test_prelude.hpp"

namespace first_ge {
#include "lib/data_structure/segment_tree/first_element_atleast_x.cpp"
}
namespace kth_one {
#include "lib/data_structure/segment_tree/kth_one_list_removal.cpp"
}
namespace max_subarray {
#include "lib/data_structure/segment_tree/max_sub_array_sum.cpp"
}
namespace max_prefix {
#include "lib/data_structure/segment_tree/max_prefix_in_a_to_b.cpp"
}
// All five pasted into one solution: compiling this is the test (they used to all be `class seg_tree`).
namespace together {
#include "lib/data_structure/segment_tree/seg_tree/seg_tree.cpp"
#include "lib/data_structure/segment_tree/first_element_atleast_x.cpp"
#include "lib/data_structure/segment_tree/kth_one_list_removal.cpp"
#include "lib/data_structure/segment_tree/max_sub_array_sum.cpp"
#include "lib/data_structure/segment_tree/max_prefix_in_a_to_b.cpp"
}

TEST(SegTreeApps, FirstIndexAtLeastX) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<ll> a = g.vec(n, -20, 20);
            const vector<ll> original = a;
            first_ge::first_atleast_seg_tree t(a);
            ASSERT_EQ(a, original);  // the caller's vector is no longer resized
            for (int q = 0; q < 100; q++) {
                if (g.coin()) {
                    int i = g.range(0, n - 1);
                    a[i] = g.range(-20, 20);
                    t.update(i, a[i]);
                } else {
                    ll x = g.range(-25, 25);
                    int l = g.range(0, n - 1);
                    ll want = -1;
                    for (int j = l; j < n; j++)
                        if (a[j] >= x) { want = j; break; }
                    ASSERT_EQ(t.query(x, l), want) << seed << " n=" << n << " x=" << x << " l=" << l;
                }
            }
        }
    }
}

TEST(SegTreeApps, KthOneWithRemovals) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            kth_one::kth_one_seg_tree t(n);
            vector<ll> alive(n);
            iota(alive.begin(), alive.end(), 0);
            while (!alive.empty()) {  // classic "remove the k-th remaining element"
                int k = g.range(1, alive.size());
                ASSERT_EQ(t.query(k), alive[k - 1]) << seed << " " << n;
                t.update(alive[k - 1], 0);
                alive.erase(alive.begin() + k - 1);
            }
        }
    }
}

TEST(SegTreeApps, MaxSubarraySumAllowsEmpty) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<ll> a = g.vec(n, -10, 10);
            max_subarray::max_subarray_seg_tree t(a);
            for (int q = 0; q < 100; q++) {
                if (g.coin()) {
                    int i = g.range(0, n - 1);
                    a[i] = g.range(-10, 10);
                    t.update(i, a[i]);
                } else {
                    auto [l, r] = g.segment(n);
                    ll best = 0, cur = 0;  // Kadane with empty subarray allowed
                    for (int i = l; i <= r; i++) cur = max(0LL, cur + a[i]), best = max(best, cur);
                    ASSERT_EQ(t.query(l, r), best) << seed << " " << n;
                }
            }
        }
    }
}

TEST(SegTreeApps, MaxPrefixSumInRange) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<ll> a = g.vec(n, -10, 10);
            max_prefix::max_prefix_seg_tree t(a);
            for (int q = 0; q < 100; q++) {
                if (g.coin()) {
                    int i = g.range(0, n - 1);
                    a[i] = g.range(-10, 10);
                    t.update(i, i, a[i]);
                } else {
                    auto [l, r] = g.segment(n);
                    ll sum = 0, best = 0;  // empty prefix allowed
                    for (int i = l; i <= r; i++) sum += a[i], best = max(best, sum);
                    auto [s, p] = t.query(l, r);
                    ASSERT_EQ(s, sum) << seed << " " << n;
                    ASSERT_EQ(p, best) << seed << " " << n;
                }
            }
        }
    }
}

TEST(SegTreeApps, EdgeCases) {
    vector<ll> a{-5};
    first_ge::first_atleast_seg_tree t(a);
    EXPECT_EQ(t.query(-5, 0), 0);
    EXPECT_EQ(t.query(-4, 0), -1);  // none

    kth_one::kth_one_seg_tree one(1);
    EXPECT_EQ(one.query(1), 0);

    vector<ll> neg{-3, -1, -2};
    EXPECT_EQ(max_subarray::max_subarray_seg_tree(neg).query(0, 2), 0);  // empty subarray
}
