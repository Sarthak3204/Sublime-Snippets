// @aliases BinaryLifting
// Binary lifting.   binary_lifting t(root, n, adj);
//   t.kth_ancestor(k, v) (root if k > depth[v]); t.get_lca(a, b); t.get_distance(a, b);
//   t.is_ancestor(a, b); t.on_path(x, a, b); t.kth_node_on_path(k, a, b)
class binary_lifting {
public:
    int log;
    vector<int> depth, tin, tout, parent;
    vector<vector<int>> up;

    binary_lifting(int root, int n, vector<int> adj[]) : log(32 - __builtin_clz(n)), depth(n), tin(n), tout(n), parent(n) {
        up.assign(log, vector<int>(n, root));
        dfs(root, -1, adj);
    }

    int kth_ancestor(int k, int v) const {
        for (int i = log - 1; i >= 0; i--)
            if ((k >> i) & 1) v = up[i][v];
        return v;
    }

    int get_lca(int a, int b) const {
        if (depth[a] < depth[b]) swap(a, b);

        a = kth_ancestor(depth[a] - depth[b], a);
        if (a == b) return a;

        for (int i = log - 1; i >= 0; i--) {
            if (up[i][a] != up[i][b]) {
                a = up[i][a];
                b = up[i][b];
            }
        }
        return up[0][a];
    }

    int get_distance(int a, int b) const { return depth[a] + depth[b] - 2 * depth[get_lca(a, b)]; }
    bool is_ancestor(int a, int b) const { return tin[a] <= tin[b] and tout[b] <= tout[a]; }  // a is ancestor of b
    bool on_path(int x, int a, int b) const {  // x on the path from a to b
        return (is_ancestor(x, a) or is_ancestor(x, b)) and is_ancestor(get_lca(a, b), x);
    }

    int kth_node_on_path(int k, int a, int b) const {  // 0 <= k <= distance(a, b)
        int c = get_lca(a, b);
        int lhs = depth[a] - depth[c];
        int rhs = depth[b] - depth[c];
        assert(0 <= k and k <= lhs + rhs);
        if (k < lhs) return kth_ancestor(k, a);
        return kth_ancestor(lhs + rhs - k, b);
    }

private:
    void dfs(int v, int p, vector<int> adj[]) {
        tin[v] = ++timer_;
        parent[v] = p;

        for (auto &u : adj[v]) {
            if (u == p) continue;
            depth[u] = depth[v] + 1;

            up[0][u] = v;
            for (int i = 1; i < log; i++) up[i][u] = up[i - 1][up[i - 1][u]];

            dfs(u, v, adj);
        }

        tout[v] = ++timer_;
    }

    int timer_ = -1;
};
