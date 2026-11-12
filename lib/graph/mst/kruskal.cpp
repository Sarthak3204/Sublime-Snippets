// @aliases mst_kruskal
// @needs dsu
auto kruskal = [&]() -> int {
    sort(edges.begin(), edges.end(), [&](const auto & lhs, const auto & rhs) {
        return lhs[2] < rhs[2];
    });

    dsu d(n);

    int cnt = 0, ans = 0;
    for (auto &[u, v, wt] : edges) {
        if (d.find(u) != d.find(v)) {
            cnt++, ans += wt;
            d.unite(u, v);
        }
    }

    return (cnt == n - 1 ? ans : -1);
};
