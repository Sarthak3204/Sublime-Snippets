vector<int> dist(n, INF);

queue<int> q;
q.push(0);
dist[0] = 0;

while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (auto &u : adj[v]) {
        if (dist[u] < INF) continue;
        dist[u] = dist[v] + 1;
        q.push(u);
    }
}
