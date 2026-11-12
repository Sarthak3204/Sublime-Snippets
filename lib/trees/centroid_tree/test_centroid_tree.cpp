#include "test_prelude.hpp"

// CentroidTree @needs trees_lca: the library's lca is pasted first, exactly as the plugin does.
namespace impl {
#include "lib/trees/lca/lca.cpp"
#include "lib/trees/centroid_tree/centroid_tree.cpp"
}  // namespace impl

// Nearest painted ("black") vertex: update(v) paints v, query(v) = distance to the nearest painted vertex.
TEST(CentroidTree, NearestPaintedVertexMatchesBfs) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            auto t = g.tree(n);
            vector<vector<int>> adj(n);
            for (int v = 0; v < n; v++)
                for (ll u : t[v]) adj[v].push_back(u);
            impl::CentroidTree ct(0, n, adj.data());
            vector<int> black;
            for (int op = 0; op < 3 * n; op++) {
                int v = g.range(0, n - 1);
                if (g.coin()) {
                    ct.update(v), black.push_back(v);
                } else {
                    vector<ll> d(n, INF);  // multi-source BFS from the painted vertices
                    queue<int> q;
                    for (int b : black) d[b] = 0, q.push(b);
                    while (!q.empty()) {
                        int x = q.front();
                        q.pop();
                        for (int u : adj[x])
                            if (d[u] == INF) d[u] = d[x] + 1, q.push(u);
                    }
                    if (black.empty()) {
                        ASSERT_GE(ct.query(v), INF);  // nothing painted yet
                    } else {
                        ASSERT_EQ(ct.query(v), d[v]) << "seed=" << seed << " n=" << n << " v=" << v;
                    }
                }
            }
            for (int a = 0; a < n; a++)  // the embedded LCA agrees with the brute-force tree
                for (int b = 0; b < n; b++) {
                    tu::BruteTree bt(t);
                    ASSERT_EQ(ct.L.get_distance(a, b), bt.depth[a] + bt.depth[b] - 2 * bt.depth[bt.lca(a, b)]);
                }
        }
    }
}
