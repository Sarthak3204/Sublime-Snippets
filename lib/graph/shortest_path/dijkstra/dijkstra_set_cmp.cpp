// @aliases shortest_path_dijkstra_set_cmp
vector<int> dist(n, INF), parent(n, -1);
auto dijkstra = [&](int source) {
    auto cmp = [&](const auto & lhs, const auto & rhs) {
        if (dist[lhs] != dist[rhs]) return dist[lhs] < dist[rhs];
        return lhs < rhs;
    };

    dist[source] = 0;
    set<int, decltype(cmp)> s(cmp);
    s.insert(source);

    while (!s.empty()) {
        int v = *s.begin();
        s.erase(s.begin());

        for (auto &[u, wt] : adj[v]) {
            if (dist[v] + wt < dist[u]) {
                s.erase(u);

                dist[u] = dist[v] + wt;
                parent[u] = v;

                s.insert(u);
            }
        }
    }
};
