// @aliases Topological_Sort_Dfs
function<vector<int>()> Topological_Sort_Dfs = [&]() {
	vector<int> topo, vis(n, 0);

	function<void(int)> Dfs_Topo = [&](int v) {
		vis[v] = 1;
		for (auto &u : adj[v]) {
			if (vis[u]) continue;
			Dfs_Topo(u);
		}
		topo.push_back(v);
	};

	for (int i = 0; i < n; i++) {
		if (!vis[i]) Dfs_Topo(i);
	}

	reverse(all(topo));
	return topo;
};
