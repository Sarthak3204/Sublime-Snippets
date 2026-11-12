#include "test_prelude.hpp"

namespace impl {
#include "lib/data_structure/dsu/dsu.cpp"
}
namespace rb {
#include "lib/data_structure/dsu/dsu_rollback.cpp"
}

// Brute force: explicit component labels.
struct Labels {
    vector<int> id;
    explicit Labels(int n) : id(n) { iota(id.begin(), id.end(), 0); }
    bool unite(int a, int b) {
        int x = id[a], y = id[b];
        if (x == y) return false;
        for (auto &v : id)
            if (v == y) v = x;
        return true;
    }
    int components() const { return set<int>(id.begin(), id.end()).size(); }
    int size_of(int a) const { return count(id.begin(), id.end(), id[a]); }
};

TEST(Dsu, MatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            impl::dsu d(n);
            Labels b(n);
            for (int op = 0; op < 3 * n + 5; op++) {
                int x = g.range(0, n - 1), y = g.range(0, n - 1);
                ASSERT_EQ(d.unite(x, y), b.unite(x, y));
                ASSERT_TRUE(d.same(x, y));
                int u = g.range(0, n - 1), v = g.range(0, n - 1);
                ASSERT_EQ(d.same(u, v), b.id[u] == b.id[v]);
                ASSERT_EQ(d.size(u), b.size_of(u));
                ASSERT_EQ(d.components, b.components());
            }
        }
    }
}

TEST(DsuRollback, UniteAndRollbackMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            rb::dsu_rollback d(n);
            vector<Labels> history{Labels(n)};  // state after each successful unite
            for (int op = 0; op < 4 * n + 5; op++) {
                if (g.range(0, 2) == 0) {
                    d.rollback();
                    if (history.size() > 1) history.pop_back();  // rollback of nothing is a no-op
                } else {
                    int x = g.range(0, n - 1), y = g.range(0, n - 1);
                    Labels next = history.back();
                    bool merged = next.unite(x, y);
                    ASSERT_EQ(d.unite(x, y), merged);
                    if (merged) history.push_back(next);
                }
                const Labels &b = history.back();
                ASSERT_EQ(d.components, b.components()) << seed << " " << n << " op " << op;
                for (int u = 0; u < n; u++) {
                    ASSERT_EQ(d.same(u, 0), b.id[u] == b.id[0]);
                    ASSERT_EQ(d.size(u), b.size_of(u));
                }
            }
        }
    }
}

TEST(Dsu, EdgeCases) {
    impl::dsu one(1);
    EXPECT_FALSE(one.unite(0, 0));
    EXPECT_EQ(one.components, 1);

    impl::dsu chain(200000);
    for (int i = 1; i < 200000; i++) chain.unite(i - 1, i);
    EXPECT_EQ(chain.components, 1);
    EXPECT_EQ(chain.size(123), 200000);

    rb::dsu_rollback r(3);
    r.rollback();  // empty history
    EXPECT_EQ(r.components, 3);
}
