#include "test_prelude.hpp"

namespace plain {
#include "lib/trees/hld/hld.cpp"
}
namespace seg {
#include "lib/trees/hld/hld_segment_tree.cpp"
}
namespace forest {
#include "lib/trees/hld/hld_segment_tree_forest.cpp"
}
namespace lazy {
#include "lib/trees/hld/hld_lazy_segment_tree.cpp"
}

const auto kMax = [](ll x, ll y) { return max(x, y); };

// hld_segment_tree: point assign + path max (negative values too) and the navigation helpers; hld (plain)
// must give the same navigation answers.
TEST(Hld, PathMaxAndNavigationMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            auto adj = g.tree(n, seed % 5 == 0);
            tu::BruteTree b(adj);
            vector<ll> val = g.vec(n, -1000, 1000);
            auto adj1 = adj, adj2 = adj;  // constructors reorder adjacency lists
            seg::hld_segment_tree h(0, n, adj1.data(), val, LLONG_MIN, kMax);
            plain::hld p(0, n, adj2.data());
            for (int q = 0; q < 100; q++) {
                int x = g.range(0, n - 1), y = g.range(0, n - 1);
                const auto path = b.path(x, y);
                switch (g.range(0, 3)) {
                case 0: {
                    int v = g.range(0, n - 1);
                    val[v] = g.range(-1000, 1000);
                    h.update(v, val[v]);
                    break;
                }
                case 1: {
                    ll best = LLONG_MIN;
                    for (ll v : path) best = max(best, val[v]);
                    ASSERT_EQ(h.query(x, y), best) << seed << " " << n;
                    break;
                }
                case 2: {
                    ASSERT_EQ(h.get_lca(x, y), b.lca(x, y));
                    ASSERT_EQ(p.get_lca(x, y), b.lca(x, y));
                    ASSERT_EQ(h.get_distance(x, y), (ll)path.size() - 1);
                    ASSERT_EQ(p.is_ancestor(x, y), b.lca(x, y) == x);
                    int j = g.range(0, path.size() - 1);
                    ASSERT_EQ(h.kth_node_on_path(j, x, y), path[j]);
                    ASSERT_EQ(p.kth_node_on_path(j, x, y), path[j]);
                    ASSERT_EQ(p.kth_node_on_path(path.size(), x, y), -1);
                    break;
                }
                default: {
                    int k = g.range(0, b.depth[x] + 1);  // one past the root -> -1
                    ll up = x;
                    for (int i = 0; i < k && up != -1; i++) up = b.parent[up];
                    ASSERT_EQ(h.kth_ancestor(k, x), up);
                    ASSERT_EQ(p.kth_ancestor(k, x), up);
                }
                }
            }
        }
    }
}

TEST(Hld, LazyPathAddPathSum) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            auto adj = g.tree(n);
            tu::BruteTree b(adj);
            vector<ll> val = g.vec(n, -100, 100);
            auto adj1 = adj;
            lazy::hld_lazy_segment_tree<ll> h(0, n, adj1.data(), val);
            for (int q = 0; q < 100; q++) {
                int x = g.range(0, n - 1), y = g.range(0, n - 1);
                const auto path = b.path(x, y);
                if (g.coin()) {
                    ll d = g.range(-100, 100);
                    for (ll v : path) val[v] += d;
                    h.update(x, y, d);
                } else {
                    ll want = 0;
                    for (ll v : path) want += val[v];
                    ASSERT_EQ(h.query(x, y), want) << seed << " " << n;
                }
            }
        }
    }
}

TEST(Hld, ForestMatchesPerComponentBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        // Two trees: vertices [0, n1) and [n1, n1 + n2).
        int n1 = g.range(1, 12), n2 = g.range(1, 12), n = n1 + n2;
        auto t1 = g.tree(n1), t2 = g.tree(n2);
        vector<vector<ll>> adj(n);
        for (int v = 0; v < n1; v++) adj[v] = t1[v];
        for (int v = 0; v < n2; v++)
            for (ll u : t2[v]) adj[n1 + v].push_back(n1 + u);
        tu::BruteTree b1(t1), b2(t2);
        vector<ll> val = g.vec(n, -50, 50);
        forest::hld_segment_tree_forest h(n, adj.data(), val, LLONG_MIN, kMax);
        for (int q = 0; q < 50; q++) {
            bool first = g.coin();
            int base = first ? 0 : n1, sz = first ? n1 : n2;
            const auto &b = first ? b1 : b2;
            int x = g.range(0, sz - 1), y = g.range(0, sz - 1);
            ll best = LLONG_MIN;
            for (ll v : b.path(x, y)) best = max(best, val[base + v]);
            ASSERT_EQ(h.query(base + x, base + y), best) << seed;
            ASSERT_EQ(h.get_lca(base + x, base + y), base + b.lca(x, y)) << seed;
        }
    }
}

TEST(Hld, EdgeValuesSkipTheLca) {
    // Edge weights stored on the child: path query must exclude the LCA's own value.
    //      0
    //    /   \       w(0-1)=5, w(0-2)=7, w(1-3)=2
    //   1     2
    //   |
    //   3
    vector<vector<ll>> adj{{1, 2}, {0, 3}, {0}, {1}};
    vector<ll> on_child{100, 5, 7, 2};  // node 0's value must never be counted
    seg::hld_segment_tree h(0, 4, adj.data(), on_child, LLONG_MIN, kMax, true);
    EXPECT_EQ(h.query(3, 2), 7);
    EXPECT_EQ(h.query(3, 1), 2);
    EXPECT_EQ(h.query(0, 0), LLONG_MIN);  // empty edge path -> identity
}
