// @aliases CentroidTree
// @needs trees_lca
class CentroidTree {
public:
    vector<int> siz, removed, centroid_parent, best;
    lca L;

    CentroidTree(int root, int n, vector<int> adj[]) :
        siz(n), removed(n), centroid_parent(n, -1), best(n, INF), L(root, n, adj) {//remeber to change this; fix: -1 = root of the centroid tree (was 0: query looped forever)
        dfs3(root, adj);
    }

    void dfs1(int v, int p, vector<int> adj[]) {//find siz
        siz[v] = 1;
        for (auto &u : adj[v]) {
            if (u == p or removed[u]) continue;
            dfs1(u, v, adj);
            siz[v] += siz[u];
        }
    }

    int dfs2(int v, int p, vector<int> adj[], int m) {//find centroid for a tree
        for (auto &u : adj[v]) {
            if (u == p or removed[u]) continue;
            if (siz[u] > (m >> 1)) return dfs2(u, v, adj, m);
        }
        return v;
    }

    int dfs3(int v, vector<int> adj[]) {//assign centroid parent
        dfs1(v, -1, adj);
        int c = dfs2(v, -1, adj, siz[v]);
        removed[c] = 1;

        for (auto &u : adj[c]) {
            if (removed[u]) continue;
            centroid_parent[dfs3(u, adj)] = c;
        }
        return c;
    }
    //nearest black node
    int query(int v) {
        int ans = best[v], curr = centroid_parent[v];

        while (curr != -1) {
            ans = min(ans, best[curr] + L.get_distance(curr, v));
            curr = centroid_parent[curr];
        }
        return ans;
    }

    void update(int v) {
        best[v] = 0;
        int curr = centroid_parent[v];
        while (curr != -1) {
            best[curr] = min(best[curr], L.get_distance(curr, v));
            curr = centroid_parent[curr];
        }
    }
};
