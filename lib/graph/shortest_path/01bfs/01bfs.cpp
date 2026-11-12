vector<pair<int, int>> adj[n];
for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    u--; v--;
    adj[u].push_back({v, 0});
    adj[v].push_back({u, 1});//reverse edge of wt 1
}

vector<int> dist(n, INF);
dist[s] = 0;//source

deque<int> q;
q.push_back(s);

while (!q.empty()) {
    int v = q.front();
    q.pop_front();

    for (auto &[u, wt] : adj[v]) {
        int now = dist[v] + wt;
        if (now < dist[u]) {
            dist[u] = now;
            if (wt) q.push_back(u);
            else q.push_front(u);
        }
    }
}
