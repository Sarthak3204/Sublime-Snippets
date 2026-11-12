// @aliases HLD_segment_tree_forest
// hld_segment_tree for a forest: every component is decomposed (rooted at its smallest vertex).
//   hld_segment_tree_forest h(n, adj, val, LLONG_MIN, [](ll x, ll y) { return max(x, y); });
//   a and b must be in the same tree for path queries / LCA.
// Edge values (val_edges = true): store w(u, parent(u)) at u; path queries then skip the LCA.
// Note: the constructor reorders adj (heavy child first).
template <class T, class Merge>
class hld_segment_tree_forest {
public:
    vector<int> tin, tout, depth, parent, subtree, tour_list, tour_start, heavy_root, euler;

    hld_segment_tree_forest(int n, vector<int> adj[], const vector<T> &arr, T identity_element, Merge merge,
                            bool val_edges = false)
        : tin(n), tout(n), depth(n), parent(n), subtree(n), tour_list(n), tour_start(n), heavy_root(n), identity_element_(identity_element), merge_(merge), val_edges_(val_edges) {
        first_.assign(n, 0);
        vector<int> vis(n), roots;
        for (int i = 0; i < n; i++) {
            if (vis[i]) continue;
            roots.push_back(i);
            dfs(i, -1, vis, adj);
        }
        for (int r : roots) dfs_tour(r, false, adj);
        st_ = euler_sparse_table(euler, depth);
        seg_ = seg_tree(tour_list, arr, identity_element_, merge_);
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

    T query(int a, int b) const {  // merge over the path a - b (merge must be commutative)
        T ans = identity_element_;
        for (; heavy_root[a] != heavy_root[b]; a = parent[heavy_root[a]]) {
            if (depth[heavy_root[a]] < depth[heavy_root[b]]) swap(a, b);
            ans = merge_(ans, seg_.query(tour_start[heavy_root[a]], tour_start[a]));
        }
        if (depth[a] > depth[b]) swap(a, b);
        ans = merge_(ans, seg_.query(tour_start[a] + val_edges_, tour_start[b]));
        return ans;
    }

    void update(int v, T x) { seg_.update(tour_start[v], x); }

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

    class seg_tree {  // iterative, inclusive [lo, hi]
    public:
        seg_tree() {}
        seg_tree(const vector<int> &tour_list, const vector<T> &arr, T identity_element, Merge merge)
            : identity_element_(identity_element), merge_(merge) {
            int n = tour_list.size();
            while (s_ < n) s_ <<= 1;
            tree_.assign(s_ << 1, identity_element_);
            for (int i = 0; i < n; i++) tree_[s_ + i] = arr[tour_list[i]];
            for (int i = s_ - 1; i >= 1; i--) tree_[i] = merge_(tree_[i << 1], tree_[i << 1 | 1]);
        }
        void update(int i, T val) {
            apply(s_ + i, val);
            for (int j = (s_ + i) >> 1; j >= 1; j >>= 1) tree_[j] = merge_(tree_[j << 1], tree_[j << 1 | 1]);
        }
        T query(int lo, int hi) const {
            T ans = identity_element_;
            for (lo += s_, hi += s_; lo <= hi; lo >>= 1, hi >>= 1) {
                if (lo & 1) ans = merge_(ans, tree_[lo++]);
                if (!(hi & 1)) ans = merge_(ans, tree_[hi--]);
            }
            return ans;
        }

    private:
        void apply(int node, T val) { tree_[node] = val; }  // remember to change: assignment by default

        vector<T> tree_;
        int s_ = 1;
        T identity_element_{};
        Merge merge_{};
    };

    void dfs(int v, int p, vector<int> &vis, vector<int> adj[]) {
        vis[v] = 1;
        parent[v] = p, subtree[v] = 1;
        depth[v] = p < 0 ? 0 : depth[p] + 1;

        for (auto &u : adj[v]) {
            if (u == p) continue;
            dfs(u, v, vis, adj);
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
    T identity_element_;
    Merge merge_;
    bool val_edges_;
    euler_sparse_table st_;
    seg_tree seg_;
};
