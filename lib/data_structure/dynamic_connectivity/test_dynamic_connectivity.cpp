#include "test_prelude.hpp"

// dynamic_connectivity @needs dsu_rollback: pasted first, exactly as the plugin does.
namespace impl {
#include "lib/data_structure/dsu/dsu_rollback.cpp"
#include "lib/data_structure/dynamic_connectivity/dynamic_connectivity.cpp"
}  // namespace impl

int brute_components(int n, const vector<pair<int, int>> &edges) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
    int c = n;
    for (auto [a, b] : edges)
        if (f(a) != f(b)) p[f(a)] = f(b), c--;
    return c;
}

TEST(DynamicConnectivity, MatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            int q = g.range(1, 40);
            struct life { int a, b, l, r; };
            vector<life> edges;
            int m = g.range(0, 2 * n);
            for (int i = 0; i < m; i++) {
                auto [l, r] = g.segment(q);
                edges.push_back({(int)g.range(0, n - 1), (int)g.range(0, n - 1), (int)l, (int)r});
            }
            impl::Dynamic_Connectivity dc(q, n);
            for (auto e : edges) dc.update(e.a, e.b, e.l, e.r);  // edge present at times [l, r]
            dc.dfs();
            for (int t = 0; t < q; t++) {
                vector<pair<int, int>> alive;
                for (auto e : edges)
                    if (e.l <= t and t <= e.r) alive.push_back({e.a, e.b});
                ASSERT_EQ(dc.ans[t], brute_components(n, alive)) << "seed=" << seed << " n=" << n << " t=" << t;
            }
        }
    }
}

TEST(DynamicConnectivity, EdgeCases) {
    impl::Dynamic_Connectivity dc(1, 3);
    dc.update(0, 1, 1, 0);  // empty interval: ignored
    dc.dfs();
    EXPECT_EQ(dc.ans[0], 3);
}
