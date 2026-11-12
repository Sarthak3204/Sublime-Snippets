// @aliases shortest_path_dijkstra_priority_queue
vector<int> dist(n, INF), parent(n, -1);
auto dijkstra = [&] (int source) {
    dist[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    q.push({0, source});

    while (!q.empty()) {
        auto [d, v] = q.top();
        q.pop();

        if (d > dist[v]) continue;
        for (auto &[u, wt] : adj[v]) {
            if (d + wt < dist[u]) {
                dist[u] = d + wt;
                parent[u] = v;

                q.push({dist[u], u});
            }
        }
    }
};
