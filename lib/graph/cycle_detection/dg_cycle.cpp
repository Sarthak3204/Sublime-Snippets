// cycle check in directed graph
vector<int> vis1(N, 0);//normal vis
vector<int> vis2(N, 0);//dfs vis
bool DG_cycle(int v) {
	vis1[v] = 1;
	vis2[v] = 1;
	for (auto &u : adj[v]) {
		if (!vis1[u]) {
			if (DG_cycle(u)) return true;
		}
		else if (vis1[u] && vis2[u]) return true;
	}
	vis2[v] = 0;
	return false;
}
