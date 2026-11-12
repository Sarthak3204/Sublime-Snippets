// @aliases shortest_path_dijkstra_set
vector<int> dist(n, INF), parent(n, -1);
auto dijkstra = [&](int source) {
    dist[source] = 0;
    set<pair<int, int>> s;
    s.insert({0, source});

    while (!s.empty()) {
        auto [d, v] = *s.begin();
        s.erase(s.begin());

        for (auto &[u, wt] : adj[v]) {
            if (d + wt < dist[u]) {
                s.erase({dist[u], u});

                dist[u] = d + wt;
                parent[u] = v;

                s.insert({dist[u], u});
            }
        }
    }
};
