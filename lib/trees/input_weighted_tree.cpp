vector<pair<int, int>> adj[n];
for (int i = 1; i < n; i++) {
    int u, v, wt;
    cin >> u >> v >> wt;
    u--, v--;
    adj[u].emplace_back(v, wt);
    adj[v].emplace_back(u, wt);
}
