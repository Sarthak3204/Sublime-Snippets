// @trigger trees_lca
// @aliases LCA
// O(1) LCA with an Euler tour + sparse table.
//   lca t(root, n, adj);  t.get_lca(a, b); t.get_distance(a, b); t.is_ancestor(a, b); t.on_path(x, a, b);
class lca {
public:
    vector<int> depth, euler, tin, tout, parent;

    lca(int root, int n, vector<int> adj[]) : depth(n), tin(n), tout(n), parent(n) {
        dfs(root, -1, adj, 0);
        st_ = euler_sparse_table(euler, depth);
    }

    int get_lca(int a, int b) const {
        int l = tin[a], r = tin[b];
        if (l > r) swap(l, r);
        return st_.query(l, r, depth);
    }
    int get_distance(int a, int b) const { return depth[a] + depth[b] - 2 * depth[get_lca(a, b)]; }
    bool is_ancestor(int a, int b) const { return tin[a] <= tin[b] and tout[b] <= tout[a]; }  // a is ancestor of b
    bool on_path(int x, int a, int b) const {  // x on the path from a to b
        return (is_ancestor(x, a) or is_ancestor(x, b)) and is_ancestor(get_lca(a, b), x);
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

    void dfs(int v, int p, vector<int> adj[], int d) {
        tin[v] = timer_++;
        euler.push_back(v);
        depth[v] = d, parent[v] = p;

        for (auto &u : adj[v]) {
            if (u == p) continue;
            dfs(u, v, adj, d + 1);
            euler.push_back(v);
        }

        tout[v] = timer_++;
    }

    int timer_ = 0;
    euler_sparse_table st_;
};
