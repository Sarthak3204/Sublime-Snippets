#include "test_prelude.hpp"

// The shortest-path snippets are statement-level: they use the caller's `n`, `adj` / `edges` / `dist`.
// Each is pasted into its own function, exactly as it would be pasted into main().
using wadj = vector<vector<pair<int, int>>>;  // adj[v] = {(to, weight)}
using edge_list = vector<array<int, 3>>;      // {from, to, weight}

vector<ll> dijkstra_pq(int n, const wadj &adj, int src) {
#include "lib/graph/shortest_path/dijkstra/dijkstra_priority_queue.cpp"
    dijkstra(src);
    return dist;
}
vector<ll> dijkstra_set(int n, const wadj &adj, int src) {
#include "lib/graph/shortest_path/dijkstra/dijkstra_set.cpp"
    dijkstra(src);
    return dist;
}
vector<ll> dijkstra_set_cmp(int n, const wadj &adj, int src) {
#include "lib/graph/shortest_path/dijkstra/dijkstra_set_cmp.cpp"
    dijkstra(src);
    return dist;
}
// Returns (distances from 0, negative cycle reachable from 0?).
pair<vector<ll>, bool> bellman_ford(int n, edge_list edges) {
#include "lib/graph/shortest_path/bellman_ford/bellman_ford.cpp"
    return {dist, changed != -1};
}
vector<vector<ll>> floyd_warshall(int n, vector<vector<ll>> dist) {
#include "lib/graph/shortest_path/floyd_warshall/floyd_warshall.cpp"
    floyd();
    return dist;
}

// Brute force: relax every edge n times (no early exit), INF = unreachable.
vector<ll> brute_dist(int n, const edge_list &edges, int src) {
    vector<ll> d(n, INF);
    d[src] = 0;
    for (int it = 0; it < n; it++)
        for (auto [u, v, w] : edges)
            if (d[u] != INF) d[v] = min(d[v], d[u] + w);
    return d;
}
bool brute_neg_cycle_from0(int n, const edge_list &edges) {
    auto d = brute_dist(n, edges, 0);
    for (auto [u, v, w] : edges)
        if (d[u] != INF and d[u] + w < d[v]) return true;
    return false;
}

edge_list random_edges(tu::Gen &g, int n, int lo, int hi) {
    edge_list e;
    int m = g.range(0, 3 * n);
    for (int i = 0; i < m; i++) e.push_back({(int)g.range(0, n - 1), (int)g.range(0, n - 1), (int)g.range(lo, hi)});
    return e;
}

TEST(ShortestPath, DijkstraVariantsMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            auto edges = random_edges(g, n, 0, 20);
            wadj adj(n);
            for (auto [u, v, w] : edges) adj[u].push_back({v, w});
            int src = g.range(0, n - 1);
            auto want = brute_dist(n, edges, src);
            ASSERT_EQ(dijkstra_pq(n, adj, src), want) << "seed=" << seed << " n=" << n;
            ASSERT_EQ(dijkstra_set(n, adj, src), want) << "seed=" << seed << " n=" << n;
            ASSERT_EQ(dijkstra_set_cmp(n, adj, src), want) << "seed=" << seed << " n=" << n;
        }
    }
}

TEST(ShortestPath, BellmanFordMatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            auto edges = random_edges(g, n, -5, 20);
            auto [dist, cycle] = bellman_ford(n, edges);
            bool want_cycle = brute_neg_cycle_from0(n, edges);
            ASSERT_EQ(cycle, want_cycle) << "seed=" << seed << " n=" << n;
            if (!want_cycle) {
                ASSERT_EQ(dist, brute_dist(n, edges, 0)) << "seed=" << seed << " n=" << n;
            }
        }
    }
}

TEST(ShortestPath, FloydWarshallMatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            if (n > 31) continue;
            auto edges = random_edges(g, n, 0, 20);
            vector dist(n, vector<ll>(n, INF));
            for (auto [u, v, w] : edges) dist[u][v] = min<ll>(dist[u][v], w);  // "minimize weight between edges"
            auto got = floyd_warshall(n, dist);
            for (int s = 0; s < n; s++) ASSERT_EQ(got[s], brute_dist(n, edges, s)) << "seed=" << seed << " n=" << n;
        }
    }
}

TEST(ShortestPath, EdgeCases) {
    // Unreachable vertices stay INF; a negative cycle not reachable from 0 is not reported.
    wadj adj(3);
    adj[0].push_back({1, 7});
    EXPECT_EQ(dijkstra_pq(3, adj, 0), (vector<ll>{0, 7, INF}));
    auto [d, cyc] = bellman_ford(4, {{0, 1, 2}, {2, 3, -5}, {3, 2, 1}});
    EXPECT_FALSE(cyc);
    EXPECT_EQ(d, (vector<ll>{0, 2, INF, INF}));
    auto [d2, cyc2] = bellman_ford(3, {{0, 1, 1}, {1, 2, -3}, {2, 1, 1}});
    EXPECT_TRUE(cyc2);
    // Distances beyond 32 bits (`int` is `long long` via the boilerplate).
    wadj big(3);
    big[0].push_back({1, 2'000'000'000}), big[1].push_back({2, 2'000'000'000});
    EXPECT_EQ(dijkstra_set(3, big, 0)[2], 4'000'000'000LL);
}
