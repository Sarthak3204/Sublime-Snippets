#include "test_prelude.hpp"

namespace impl {
#include "lib/data_structure/dsu/dsu.cpp"  // kruskal @needs dsu
#include "lib/graph/bipartite/check_bipartite.cpp"
#include "lib/graph/functional_graph/functional_graph.cpp"
}  // namespace impl
using impl::dsu;

namespace dg {  // dg_cycle uses globals `N` and `adj`
const int N = 70;
vector<int> adj[N];
#include "lib/graph/cycle_detection/dg_cycle.cpp"
}  // namespace dg

using graph = vector<vector<int>>;

// ---- statement-level snippets, each pasted into its own function ----
set<pair<int, int>> bridges_of(int n, const graph &adj) {
#include "lib/graph/low_link/find_bridges.cpp"
    set<pair<int, int>> r;
    for (auto [u, v] : bridges) r.insert(minmax(u, v));
    return r;
}
vector<int> cutpoints_of(int n, const graph &adj) {
#include "lib/graph/low_link/find_articulation_point.cpp"
    return cutpoint;
}
struct scc_result {
    int scc;
    vector<int> root, root_nodes;
    vector<vector<int>> adj_scc;
};
scc_result kosaraju_of(int n, const graph &adj) {
#include "lib/graph/scc/kosaraju.cpp"
    return {scc, root, root_nodes, vector<vector<int>>(adj_scc, adj_scc + n)};
}
vector<int> topo_bfs(int n, const graph &adj) {
#include "lib/graph/topological_sort/topological_sort_bfs.cpp"
    return Topological_Sort_Bfs();
}
vector<int> topo_dfs(int n, const graph &adj) {
#include "lib/graph/topological_sort/topological_sort_dfs.cpp"
    return Topological_Sort_Dfs();
}
bool dag_cycle(int n, const graph &adj) {
#include "lib/graph/topological_sort/topological_sort_bfs.cpp"  // @needs
#include "lib/graph/cycle_detection/dag_cycle.cpp"
    return DAG_Cycle();
}
int kruskal_of(int n, vector<array<int, 3>> edges) {
#include "lib/graph/mst/kruskal.cpp"
    return kruskal();
}
pair<int, vector<pair<int, int>>> prims_of(int n, const vector<vector<pair<int, int>>> &adj) {
#include "lib/graph/mst/prims.cpp"
    int w = prims();
    return {w, mst};
}

// ---- brute force ----
int components(int n, const graph &adj, int skip_vertex = -1, pair<int, int> skip_edge = {-1, -1}) {
    vector<int> seen(n);
    int c = 0;
    for (int s = 0; s < n; s++) {
        if (seen[s] or s == skip_vertex) continue;
        c++;
        vector<int> st{s};
        seen[s] = 1;
        while (!st.empty()) {
            int v = st.back();
            st.pop_back();
            for (int u : adj[v]) {
                if (u == skip_vertex or seen[u] or pair<int, int>(minmax(u, v)) == skip_edge) continue;
                seen[u] = 1, st.push_back(u);
            }
        }
    }
    return c;
}
vector<vector<int>> reach(int n, const graph &adj) {  // reach[a][b]: path a -> b (length >= 0)
    vector r(n, vector<int>(n));
    for (int v = 0; v < n; v++) {
        r[v][v] = 1;
        for (int u : adj[v]) r[v][u] = 1;
    }
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) r[i][j] |= r[i][k] & r[k][j];
    return r;
}
bool has_cycle(int n, const graph &adj) {
    auto r = reach(n, adj);
    for (int v = 0; v < n; v++)
        for (int u : adj[v])
            if (r[u][v]) return true;  // includes self-loops
    return false;
}
// Simple undirected graph (no self-loops or parallel edges).
graph random_simple_graph(tu::Gen &g, int n) {
    graph adj(n);
    set<pair<int, int>> used;
    int m = g.range(0, 2 * n);
    for (int i = 0; i < m; i++) {
        int u = g.range(0, n - 1), v = g.range(0, n - 1);
        if (u == v or !used.insert(minmax(u, v)).second) continue;
        adj[u].push_back(v), adj[v].push_back(u);
    }
    return adj;
}
graph random_digraph(tu::Gen &g, int n, bool acyclic) {
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    shuffle(perm.begin(), perm.end(), g.eng);
    graph adj(n);
    int m = g.range(0, 2 * n);
    for (int i = 0; i < m; i++) {
        int u = g.range(0, n - 1), v = g.range(0, n - 1);
        if (acyclic) {
            if (u == v) continue;
            if (u > v) swap(u, v);
            u = perm[u], v = perm[v];
        }
        adj[u].push_back(v);
    }
    return adj;
}
bool valid_topo(int n, const graph &adj, const vector<int> &topo) {
    if ((int)topo.size() != n) return false;
    vector<int> pos(n, -1);
    for (int i = 0; i < n; i++) pos[topo[i]] = i;
    for (int v = 0; v < n; v++)
        for (int u : adj[v])
            if (pos[v] >= pos[u]) return false;
    return true;
}

TEST(Graph, BridgesAndCutpointsMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            auto adj = random_simple_graph(g, n);
            int base = components(n, adj);
            set<pair<int, int>> want_bridges;
            for (int v = 0; v < n; v++)
                for (int u : adj[v])
                    if (v < u and components(n, adj, -1, {v, u}) > base) want_bridges.insert({v, u});
            ASSERT_EQ(bridges_of(n, adj), want_bridges) << "seed=" << seed << " n=" << n;

            vector<int> want_cut(n);
            for (int v = 0; v < n; v++) {
                bool isolated = adj[v].empty();
                want_cut[v] = components(n, adj, v) > base - isolated;
            }
            ASSERT_EQ(cutpoints_of(n, adj), want_cut) << "seed=" << seed << " n=" << n;
        }
    }
}

TEST(Graph, KosarajuMatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            auto adj = random_digraph(g, n, false);
            auto r = reach(n, adj);
            auto got = kosaraju_of(n, adj);
            set<int> roots;
            for (int a = 0; a < n; a++) {
                roots.insert(got.root[a]);
                for (int b = 0; b < n; b++)
                    ASSERT_EQ(got.root[a] == got.root[b], r[a][b] and r[b][a]) << "seed=" << seed << " n=" << n;
            }
            ASSERT_EQ(got.scc, (int)roots.size());
            ASSERT_EQ(set<int>(got.root_nodes.begin(), got.root_nodes.end()), roots);
            // root_nodes is in topological order of the condensation; adj_scc is its edge list.
            vector<int> pos(n, -1);
            for (int i = 0; i < (int)got.root_nodes.size(); i++) pos[got.root_nodes[i]] = i;
            for (int v = 0; v < n; v++) {
                for (int u : got.adj_scc[v]) {
                    ASSERT_NE(v, u);
                    ASSERT_LT(pos[v], pos[u]) << "seed=" << seed << " n=" << n;
                }
                for (int u : adj[v])
                    if (got.root[v] != got.root[u]) {
                        auto &out = got.adj_scc[got.root[v]];
                        ASSERT_NE(find(out.begin(), out.end(), got.root[u]), out.end());
                    }
            }
        }
    }
}

TEST(Graph, TopologicalSortAndCycleChecks) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            for (bool acyclic : {true, false}) {
                auto adj = random_digraph(g, n, acyclic);
                bool cyc = has_cycle(n, adj);
                ASSERT_EQ(dag_cycle(n, adj), cyc) << "seed=" << seed << " n=" << n;
                ASSERT_EQ(valid_topo(n, adj, topo_bfs(n, adj)), !cyc);
                if (!cyc) {
                    ASSERT_TRUE(valid_topo(n, adj, topo_dfs(n, adj))) << "seed=" << seed << " n=" << n;
                }

                for (int v = 0; v < n; v++) dg::adj[v] = adj[v];
                fill(dg::vis1.begin(), dg::vis1.end(), 0), fill(dg::vis2.begin(), dg::vis2.end(), 0);
                bool got = false;
                for (int v = 0; v < n and !got; v++)
                    if (!dg::vis1[v]) got = dg::DG_cycle(v);
                ASSERT_EQ(got, cyc) << "seed=" << seed << " n=" << n;
                for (int v = 0; v < n; v++) dg::adj[v].clear();
            }
        }
    }
}

TEST(Graph, CheckBipartiteMatchesBruteForce) {  // 1-indexed: vertices 1..n
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            if (n > 16) continue;
            auto adj0 = random_simple_graph(g, n);
            vector<vector<int>> adj(n + 1);
            for (int v = 0; v < n; v++)
                for (int u : adj0[v]) adj[v + 1].push_back(u + 1);
            bool want = false;
            for (int mask = 0; mask < (1 << n) and !want; mask++) {
                bool ok = true;
                for (int v = 0; v < n; v++)
                    for (int u : adj0[v]) ok &= ((mask >> v) & 1) != ((mask >> u) & 1);
                want = ok;
            }
            ASSERT_EQ(impl::check_bipartite(n, adj.data()), want) << "seed=" << seed << " n=" << n;
        }
    }
}

TEST(Graph, MstKruskalAndPrimsMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<array<int, 3>> edges;
            vector<vector<pair<int, int>>> wadj(n);
            int m = g.range(0, 3 * n);
            for (int i = 0; i < m; i++) {
                int u = g.range(0, n - 1), v = g.range(0, n - 1), w = g.range(0, 20);
                edges.push_back({u, v, w});
                wadj[u].push_back({v, w}), wadj[v].push_back({u, w});
            }
            // Brute force: grow from 0, always adding the cheapest edge leaving the tree.
            vector<int> in(n);
            in[0] = 1;
            int total = 0, added = 0;
            while (true) {
                int best = -1;
                for (int i = 0; i < m; i++) {
                    auto [u, v, w] = edges[i];
                    if (in[u] != in[v] and (best == -1 or w < edges[best][2])) best = i;
                }
                if (best == -1) break;
                total += edges[best][2], added++;
                in[edges[best][0]] = in[edges[best][1]] = 1;
            }
            bool connected = added == n - 1;
            ASSERT_EQ(kruskal_of(n, edges), connected ? total : -1) << "seed=" << seed << " n=" << n;
            auto [pw, mst] = prims_of(n, wadj);
            ASSERT_EQ(pw, total) << "seed=" << seed << " n=" << n;  // weight of 0's component
            ASSERT_EQ((int)mst.size(), added);
        }
    }
}

TEST(Graph, FunctionalGraphMatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            vector<int> F(n);
            for (auto &x : F) x = g.coin() ? g.range(0, n - 1) : g.range(max<int>(0, n - 3), n - 1);
            impl::functional_graph fg(F);
            for (int v = 0; v < n; v++) {
                int x = v;
                for (int k = 0; k <= 3 * n; k++, x = F[x]) ASSERT_EQ(fg.go_forward(v, k), x) << "seed=" << seed << " n=" << n;
                for (int u = 0; u < n; u++) {
                    int want = -1, y = v;
                    for (int k = 0; k <= 2 * n; k++, y = F[y])
                        if (y == u) {
                            want = k;
                            break;
                        }
                    ASSERT_EQ(fg.min_steps(v, u), want) << "seed=" << seed << " n=" << n << " " << v << "->" << u;
                }
            }
        }
    }
}

TEST(Graph, EdgeCases) {
    EXPECT_TRUE(bridges_of(1, graph(1)).empty());
    graph path{{1}, {0, 2}, {1}};
    EXPECT_EQ(bridges_of(3, path), (set<pair<int, int>>{{0, 1}, {1, 2}}));
    EXPECT_EQ(cutpoints_of(3, path), (vector<int>{0, 1, 0}));
    graph self_loop{{0}};
    EXPECT_TRUE(dag_cycle(1, self_loop));
    EXPECT_EQ(kruskal_of(1, {}), 0);
    EXPECT_EQ(kruskal_of(2, {}), -1);
}
