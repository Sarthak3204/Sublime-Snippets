#include "test_prelude.hpp"

namespace basis {
#include "lib/data_structure/linear_basis.cpp"
}
namespace venice {
#include "lib/data_structure/venice_set.cpp"
}
namespace mx {
#include "lib/data_structure/mex_changing_set.cpp"
}
namespace diff {
#include "lib/data_structure/difference_array.cpp"
}
namespace dyn {
#include "lib/data_structure/dynamic.cpp"
}

TEST(LinearBasis, SpanMaxMinAndMergeMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        int n = g.range(1, 8);
        vector<ll> a = g.vec(n, 0, 63);  // small bit width -> many dependent vectors
        basis::linear_basis lb, left, right;
        set<ll> span{0};
        for (int i = 0; i < n; i++) {
            set<ll> next = span;
            for (ll s : span) next.insert(s ^ a[i]);
            ASSERT_EQ(lb.insert(a[i]), next.size() > span.size());
            span = next;
            (i % 2 ? left : right).insert(a[i]);
        }
        ASSERT_EQ(lb.size(), __builtin_ctzll(span.size()));  // |span| = 2^rank
        ASSERT_EQ(lb.max_xor(), *span.rbegin());
        ll min_nonzero = span.size() > 1 ? *next(span.begin()) : 0;
        ASSERT_EQ(lb.min_xor(), min_nonzero);
        for (ll x = 0; x < 64; x++) {
            ASSERT_EQ(lb.contains(x), span.count(x) == 1) << seed << " x=" << x;
            ll best = 0;
            for (ll s : span) best = max(best, s ^ x);
            ASSERT_EQ(lb.max_xor(x), best);
        }
        auto merged = left + right;
        ASSERT_EQ(merged.size(), lb.size());
        ASSERT_EQ(merged.max_xor(), lb.max_xor());
    }
}

TEST(VeniceSet, AddRemoveShiftAllMatchesMultiset) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        venice::venice_set<ll> v;
        multiset<ll> ref;
        for (int op = 0; op < 300; op++) {
            int t = g.range(0, 3);
            if (t == 0 || ref.empty()) {
                ll x = g.range(-10, 10);
                v.add(x), ref.insert(x);
            } else if (t == 1) {
                ll x = *next(ref.begin(), g.range(0, ref.size() - 1));
                v.remove(x), ref.erase(ref.find(x));
            } else if (t == 2) {
                ll w = g.range(-5, 5);
                v.update_all(w);  // every element decreases by w
                multiset<ll> shifted;
                for (ll x : ref) shifted.insert(x - w);
                ref = shifted;
            }
            ASSERT_EQ(v.size(), (ll)ref.size());
            if (!ref.empty()) ASSERT_EQ(v.get_min(), *ref.begin()) << seed << " op " << op;
        }
    }
}

TEST(Mex, AddDeleteMatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        int n = g.range(1, 10);
        vector<ll> init = g.vec(n, -3, n + 2);  // negatives and values >= n are ignored
        mx::mex m(init);
        multiset<ll> ref(init.begin(), init.end());
        for (int op = 0; op < 200; op++) {
            if (g.coin() || ref.empty()) {
                ll x = g.range(-3, n + 2);
                m.add(x), ref.insert(x);
            } else {
                ll x = *next(ref.begin(), g.range(0, ref.size() - 1));
                m.del(x), ref.erase(ref.find(x));
            }
            ll want = 0;
            while (ref.count(want)) want++;
            ASSERT_EQ(m.find(), min<ll>(want, n)) << seed;  // capacity n: mex is capped at n
        }
    }
    mx::mex empty(3);
    EXPECT_EQ(empty.find(), 0);
}

TEST(DifferenceArray, RangeAddsMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<ll> a = g.vec(n, -100, 100);
            diff::difference_array<ll> d(a);
            for (int op = 0; op < 20; op++) {
                auto [l, r] = g.segment(n);
                ll x = g.range(-100, 100);
                d.update(l, r, x);
                for (int i = l; i <= r; i++) a[i] += x;
            }
            ASSERT_EQ(d.resultant(), a) << seed << " " << n;
        }
    }
}

TEST(DynamicSegTree, SparsePointAddRangeSumOneIndexed) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        const ll n = 1'000'000'000;  // coordinates up to 1e9, 1-indexed
        dyn::dynamic<ll> t(n);
        map<ll, ll> ref;
        for (int op = 0; op < 300; op++) {
            if (g.coin()) {
                ll i = g.coin() ? g.range(1, 20) : g.range(1, n), x = g.range(-100, 100);
                t.update(i, x), ref[i] += x;
            } else {
                ll l = g.range(1, n), r = g.range(1, n);
                if (g.coin()) l = g.range(1, 20), r = g.range(1, 20);
                if (l > r) swap(l, r);
                ll want = 0;
                for (auto it = ref.lower_bound(l); it != ref.end() && it->first <= r; ++it) want += it->second;
                ASSERT_EQ(t.query(l, r), want) << seed;
            }
        }
    }
}
