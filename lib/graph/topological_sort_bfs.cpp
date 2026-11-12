// @aliases Topological_Sort_Bfs
function<vector<int>()> Topological_Sort_Bfs = [&]() {
	vector<int> topo, inDegree(n);
	
	for (int v = 0; v < n; v++) {
		for (auto &u : adj[v]) {
			inDegree[u]++;
		}
	}
	
	queue<int> q;
	
	for (int i = 0; i < n; i++) {
		if (!inDegree[i]) {
			q.push(i);
		}
	}
	
	while (!q.empty()) {
		int v = q.front(); q.pop();
		topo.push_back(v);
		for (auto &u : adj[v]) {
			inDegree[u]--;
			if (!inDegree[u]) {
				q.push(u);
			}
		}
	}
	
	return topo;
};
