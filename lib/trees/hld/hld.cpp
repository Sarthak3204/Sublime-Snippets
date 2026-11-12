// @aliases HLD
// Heavy-light decomposition (no segment tree): LCA, distance, k-th ancestor, k-th node on a path.
// tour_start[v] is v's position in a heavy-first DFS order: chains and subtrees are contiguous.
//   hld h(root, n, adj);  h.get_lca(a, b); h.kth_ancestor(k, v); h.kth_node_on_path(k, a, b)
// Note: the constructor reorders adj (heavy child first).
class hld {
public:
    vector<int> tin, tout, depth, parent, subtree, tour_list, tour_start, heavy_root, euler;

    hld(int root, int n, vector<int> adj[]) : tin(n), tout(n), depth(n), parent(n), subtree(n), tour_list(n), tour_start(n), heavy_root(n) {
        first_.assign(n, 0);
        dfs(root, -1, adj);
        dfs_tour(root, false, adj);
        st_ = euler_sparse_table(euler, depth);
    }

    int get_lca(int a, int b) const {
        int l = first_[a], r = first_[b];  // first Euler position (tin only matches it in a single tree)
        if (l > r) swap(l, r);
        return st_.query(l, r, depth);
    }
    int get_distance(int a, int b) const { return depth[a] + depth[b] - 2 * depth[get_lca(a, b)]; }
    bool is_ancestor(int a, int b) const { return tin[a] <= tin[b] and tout[b] <= tout[a]; }  // a is ancestor of b
    bool on_path(int x, int a, int b) const {  // x on the path from a to b
        return (is_ancestor(x, a) or is_ancestor(x, b)) and is_ancestor(get_lca(a, b), x);
    }

    int kth_ancestor(int k, int a) const {  // -1 if k > depth[a]
        if (k > depth[a]) return -1;
        while (a >= 0) {
            int root = heavy_root[a];
            if (k <= depth[a] - depth[root]) return tour_list[tour_start[a] - k];
            k -= depth[a] - depth[root] + 1;
            a = parent[root];
        }
        return a;
    }

    int kth_node_on_path(int k, int a, int b) const {  // -1 if k is outside [0, distance(a, b)]
        int c = get_lca(a, b);
        int lhs = depth[a] - depth[c];
        int rhs = depth[b] - depth[c];
        if (0 <= k and k <= lhs + rhs) {
            if (k < lhs) return kth_ancestor(k, a);
            return kth_ancestor(lhs + rhs - k, b);
        }
        return -1;
    }

private:
    class euler_sparse_table {
    public:
        euler_sparse_table() {}
        euler_sparse_table(const vector<int> &euler, const vector<int> &depth) : n_(euler.size()), lg_(n_ + 1) {
            for (int i = 2; i <= n_; i++) lg_[i] = lg_[i >> 1] + 1;
            table_.resize(lg_[n_] + 1);
            table_[0] = euler;
            for (int j = 1; j <= lg_[n_]; j++) {
                table_[j].resize(n_ - (1 << j) + 1);
                for (int i = 0; i + (1 << j) - 1 < n_; i++) {
                    int lhs = table_[j - 1][i], rhs = table_[j - 1][i + (1 << (j - 1))];
                    table_[j][i] = depth[lhs] < depth[rhs] ? lhs : rhs;
                }
            }
        }
        int query(int l, int r, const vector<int> &depth) const {
            int j = lg_[r - l + 1];
            int lhs = table_[j][l], rhs = table_[j][r - (1 << j) + 1];
            return depth[lhs] < depth[rhs] ? lhs : rhs;
        }

    private:
        int n_ = 0;
        vector<int> lg_;
        vector<vector<int>> table_;
    };

    void dfs(int v, int p, vector<int> adj[]) {
        parent[v] = p, subtree[v] = 1;
        depth[v] = p < 0 ? 0 : depth[p] + 1;

        for (auto &u : adj[v]) {
            if (u == p) continue;
            dfs(u, v, adj);
            subtree[v] += subtree[u];
        }
        // Heavy-light subtree reordering.
        sort(adj[v].begin(), adj[v].end(), [&](int lhs, int rhs) { return subtree[lhs] > subtree[rhs]; });
    }

    void dfs_tour(int v, bool heavy, vector<int> adj[]) {
        tin[v] = timer_++;
        heavy_root[v] = heavy ? heavy_root[parent[v]] : v;
        tour_list[tour_] = v;
        tour_start[v] = tour_++;
        first_[v] = euler.size();
        euler.push_back(v);

        bool heavy_child = true;
        for (auto &u : adj[v]) {
            if (u == parent[v]) continue;
            dfs_tour(u, heavy_child, adj);
            euler.push_back(v);
            heavy_child = false;
        }

        tout[v] = timer_++;
    }

    vector<int> first_;
    int tour_ = 0, timer_ = 0;
    euler_sparse_table st_;
};
