int timer = 0;
vector<pair<int, int>> bridges;
vector<int> vis(n), tin(n), low(n);

function<void(int, int)> dfs = [&](int v, int p) {//Tarjan's algo
    vis[v] = 1;
    tin[v] = low[v] = timer++;
    for (auto &u : adj[v]) {
        if (u == p) continue;
        if (vis[u]) low[v] = min(low[v], low[u]);
        else {
            dfs(u, v);
            low[v] = min(low[v], low[u]);
            if (low[u] > tin[v])
                bridges.push_back({u, v});
        }
    }
};

for (int i = 0; i < n; i++)
    if (!vis[i]) dfs(i, -1);
