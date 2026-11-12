#include "test_prelude.hpp"

namespace euler_lca {
#include "lib/trees/lca/lca.cpp"
}
namespace lifting {
#include "lib/trees/lca/binary_lifting.cpp"
}

// Both LCA snippets answer the same questions; test them against one brute-force tree.
TEST(Lca, EulerAndBinaryLiftingMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            auto adj = g.tree(n, seed % 5 == 0);
            tu::BruteTree b(adj);
            euler_lca::lca e(0, n, adj.data());
            lifting::binary_lifting bl(0, n, adj.data());
            for (int q = 0; q < 100; q++) {
                int x = g.range(0, n - 1), y = g.range(0, n - 1), z = g.range(0, n - 1);
                const ll c = b.lca(x, y);
                const auto path = b.path(x, y);
                const ll dist = path.size() - 1;
                const bool on_path = find(path.begin(), path.end(), z) != path.end();
                const bool anc = b.lca(x, y) == x;

                ASSERT_EQ(e.get_lca(x, y), c) << seed << " " << n;
                ASSERT_EQ(e.get_distance(x, y), dist);
                ASSERT_EQ(e.is_ancestor(x, y), anc);
                ASSERT_EQ(e.on_path(z, x, y), on_path);

                ASSERT_EQ(bl.get_lca(x, y), c) << seed << " " << n;
                ASSERT_EQ(bl.get_distance(x, y), dist);
                ASSERT_EQ(bl.is_ancestor(x, y), anc);
                ASSERT_EQ(bl.on_path(z, x, y), on_path);

                int k = g.range(0, b.depth[x] + 1);  // one past the root -> root (original behaviour)
                ll up = x;
                for (int i = 0; i < k && b.parent[up] != -1; i++) up = b.parent[up];
                ASSERT_EQ(bl.kth_ancestor(k, x), up);
                int j = g.range(0, dist);
                ASSERT_EQ(bl.kth_node_on_path(j, x, y), path[j]);
            }
        }
    }
}

TEST(Lca, EdgeCases) {
    vector<vector<ll>> single(1);
    euler_lca::lca e(0, 1, single.data());
    lifting::binary_lifting bl(0, 1, single.data());
    EXPECT_EQ(e.get_lca(0, 0), 0);
    EXPECT_EQ(bl.get_lca(0, 0), 0);
    EXPECT_EQ(bl.kth_ancestor(0, 0), 0);
    EXPECT_EQ(bl.kth_node_on_path(0, 0, 0), 0);

    // Deep path 0 - 1 - ... - (n-1): recursive DFS needs a judge-sized stack (tests link with 256 MB).
    tu::Gen g(1);
    const int n = 200'000;
    auto adj = g.tree(n, true);
    tu::BruteTree b(adj);
    euler_lca::lca deep_e(0, n, adj.data());
    lifting::binary_lifting deep(0, n, adj.data());
    for (int v = 0; v < n; v += 3701)
        for (int u = 0; u < n; u += 9157) {
            ASSERT_EQ(deep.get_lca(u, v), b.depth[u] < b.depth[v] ? u : v);
            ASSERT_EQ(deep_e.get_lca(u, v), b.depth[u] < b.depth[v] ? u : v);
        }
}
