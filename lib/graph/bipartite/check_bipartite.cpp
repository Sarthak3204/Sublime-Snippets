bool check_bipartite(int n, vector<int> adj[]) {
	vector<int> color(n + 1, -1);
	for (int i = 1; i <= n; i++) {
		if (color[i] == -1) {
			queue<int> q;
			q.push(i);
			color[i] = 0;
			while (!q.empty()) {
				int v = q.front();
				q.pop();
				for (auto &u : adj[v]) {
					if (color[u] == -1) {
						color[u] = 1 - color[v];
						q.push(u);
					}
					if (color[u] == color[v]) return false;
				}
			}
		}
	}
	return true;
}
