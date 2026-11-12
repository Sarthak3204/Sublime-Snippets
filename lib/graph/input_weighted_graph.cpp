vector<pair<int, int>> adj[n];
for (int i = 0; i < m; i++) {
    int u, v, wt;
    cin >> u >> v >> wt;
    u--, v--;
    adj[u].emplace_back(v, wt);
}
