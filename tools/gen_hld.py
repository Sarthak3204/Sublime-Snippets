"""Generate lib/trees/hld/hld*.cpp (4 HLD variants sharing one decomposition). Usage: python tools/gen_hld.py lib/trees/hld"""
import sys
from pathlib import Path

out = Path(sys.argv[1])

EULER_SPARSE = """    class euler_sparse_table {
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
"""

NAV = """    int get_lca(int a, int b) const {
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
"""

DFS = """    void dfs(int v, int p, vector<int> adj[]) {
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
"""

MEMBERS = "    vector<int> tin, tout, depth, parent, subtree, tour_list, tour_start, heavy_root, euler;\n"
INIT = "tin(n), tout(n), depth(n), parent(n), subtree(n), tour_list(n), tour_start(n), heavy_root(n)"
NOTE = "// Edge values (val_edges = true): store w(u, parent(u)) at u; path queries then skip the LCA.\n"

SEG = """    class seg_tree {  // iterative, inclusive [lo, hi]
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
"""

PATH_QUERY = """    T query(int a, int b) const {  // merge over the path a - b (merge must be commutative)
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
"""


def write(name, body):
    (out / name).write_text(body, encoding="utf-8", newline="\n")
    print("wrote", name)


write("hld.cpp", f"""// @aliases HLD
// Heavy-light decomposition (no segment tree): LCA, distance, k-th ancestor, k-th node on a path.
// tour_start[v] is v's position in a heavy-first DFS order: chains and subtrees are contiguous.
//   hld h(root, n, adj);  h.get_lca(a, b); h.kth_ancestor(k, v); h.kth_node_on_path(k, a, b)
// Note: the constructor reorders adj (heavy child first).
class hld {{
public:
{MEMBERS}
    hld(int root, int n, vector<int> adj[]) : {INIT} {{
        first_.assign(n, 0);
        dfs(root, -1, adj);
        dfs_tour(root, false, adj);
        st_ = euler_sparse_table(euler, depth);
    }}

{NAV}
private:
{EULER_SPARSE}
{DFS}
    vector<int> first_;
    int tour_ = 0, timer_ = 0;
    euler_sparse_table st_;
}};
""")

write("hld_segment_tree.cpp", f"""// @aliases HLD_segment_tree
// HLD + point-update segment tree over node values. Generic value type and merge.
//   hld_segment_tree h(root, n, adj, val, LLONG_MIN, [](ll x, ll y) {{ return max(x, y); }});
//   h.query(a, b) -> merge over the path; h.update(v, x) -> val[v] = x; plus get_lca, kth_ancestor, ...
{NOTE}// Note: the constructor reorders adj (heavy child first).
template <class T, class Merge>
class hld_segment_tree {{
public:
{MEMBERS}
    hld_segment_tree(int root, int n, vector<int> adj[], const vector<T> &arr, T identity_element, Merge merge,
                     bool val_edges = false)
        : {INIT}, identity_element_(identity_element), merge_(merge), val_edges_(val_edges) {{
        first_.assign(n, 0);
        dfs(root, -1, adj);
        dfs_tour(root, false, adj);
        st_ = euler_sparse_table(euler, depth);
        seg_ = seg_tree(tour_list, arr, identity_element_, merge_);
    }}

{NAV}
{PATH_QUERY}
private:
{EULER_SPARSE}
{SEG}
{DFS}
    vector<int> first_;
    int tour_ = 0, timer_ = 0;
    T identity_element_;
    Merge merge_;
    bool val_edges_;
    euler_sparse_table st_;
    seg_tree seg_;
}};
""")

write("hld_segment_tree_forest.cpp", f"""// @aliases HLD_segment_tree_forest
// hld_segment_tree for a forest: every component is decomposed (rooted at its smallest vertex).
//   hld_segment_tree_forest h(n, adj, val, LLONG_MIN, [](ll x, ll y) {{ return max(x, y); }});
//   a and b must be in the same tree for path queries / LCA.
{NOTE}// Note: the constructor reorders adj (heavy child first).
template <class T, class Merge>
class hld_segment_tree_forest {{
public:
{MEMBERS}
    hld_segment_tree_forest(int n, vector<int> adj[], const vector<T> &arr, T identity_element, Merge merge,
                            bool val_edges = false)
        : {INIT}, identity_element_(identity_element), merge_(merge), val_edges_(val_edges) {{
        first_.assign(n, 0);
        vector<int> vis(n), roots;
        for (int i = 0; i < n; i++) {{
            if (vis[i]) continue;
            roots.push_back(i);
            dfs(i, -1, vis, adj);
        }}
        for (int r : roots) dfs_tour(r, false, adj);
        st_ = euler_sparse_table(euler, depth);
        seg_ = seg_tree(tour_list, arr, identity_element_, merge_);
    }}

{NAV}
{PATH_QUERY}
private:
{EULER_SPARSE}
{SEG}
    void dfs(int v, int p, vector<int> &vis, vector<int> adj[]) {{
        vis[v] = 1;
        parent[v] = p, subtree[v] = 1;
        depth[v] = p < 0 ? 0 : depth[p] + 1;

        for (auto &u : adj[v]) {{
            if (u == p) continue;
            dfs(u, v, vis, adj);
            subtree[v] += subtree[u];
        }}
        // Heavy-light subtree reordering.
        sort(adj[v].begin(), adj[v].end(), [&](int lhs, int rhs) {{ return subtree[lhs] > subtree[rhs]; }});
    }}

    void dfs_tour(int v, bool heavy, vector<int> adj[]) {{
        tin[v] = timer_++;
        heavy_root[v] = heavy ? heavy_root[parent[v]] : v;
        tour_list[tour_] = v;
        tour_start[v] = tour_++;
        first_[v] = euler.size();
        euler.push_back(v);

        bool heavy_child = true;
        for (auto &u : adj[v]) {{
            if (u == parent[v]) continue;
            dfs_tour(u, heavy_child, adj);
            euler.push_back(v);
            heavy_child = false;
        }}

        tout[v] = timer_++;
    }}

    vector<int> first_;
    int tour_ = 0, timer_ = 0;
    T identity_element_;
    Merge merge_;
    bool val_edges_;
    euler_sparse_table st_;
    seg_tree seg_;
}};
""")

LAZY = """    class lazy_sgt {  // recursive, inclusive [l, r]; range add + range sum: change merge/combine/apply
    public:
        lazy_sgt() {}
        lazy_sgt(const vector<int> &tour_list, const vector<T> &arr) {
            int n = tour_list.size();
            while (s_ < n) s_ <<= 1;
            tree_.assign(s_ << 1, identity_element_);
            lazy_.assign(s_ << 1, 0);
            updates_.assign(s_ << 1, identity_update_);
            for (int i = 0; i < n; i++) tree_[s_ + i] = arr[tour_list[i]];
            for (int i = s_ - 1; i >= 1; i--) tree_[i] = merge(tree_[i << 1], tree_[i << 1 | 1]);
        }
        T query(int l, int r) { return query(l, r, 1, 0, s_ - 1); }
        void update(int l, int r, T upd) { update(l, r, 1, 0, s_ - 1, upd); }
        T merge(T lhs, T rhs) const { return lhs + rhs; }  // remember to change

    private:
        void pushdown(int node, int lo, int hi) {
            if (lazy_[node]) {
                combine(node, lo, hi, updates_[node]);
                if (lo != hi) {
                    apply(node << 1, updates_[node]);
                    apply(node << 1 | 1, updates_[node]);
                }
                lazy_[node] = 0;
                updates_[node] = identity_update_;
            }
        }
        T query(int l, int r, int node, int lo, int hi) {
            pushdown(node, lo, hi);
            if (l <= lo and hi <= r) return tree_[node];
            if (lo > r or hi < l) return identity_element_;
            int mid = (lo + hi) >> 1;
            return merge(query(l, r, node << 1, lo, mid), query(l, r, node << 1 | 1, mid + 1, hi));
        }
        void update(int l, int r, int node, int lo, int hi, T upd) {
            pushdown(node, lo, hi);
            if (l <= lo and hi <= r) {
                apply(node, upd);
                pushdown(node, lo, hi);
                return;
            }
            if (lo > r or hi < l) return;
            int mid = (lo + hi) >> 1;
            update(l, r, node << 1, lo, mid, upd);
            update(l, r, node << 1 | 1, mid + 1, hi, upd);
            tree_[node] = merge(tree_[node << 1], tree_[node << 1 | 1]);
        }
        void combine(int node, int lo, int hi, T upd) { tree_[node] += (hi - lo + 1) * upd; }
        void apply(int node, T upd) {
            lazy_[node] = 1;
            updates_[node] += upd;
        }

        vector<T> tree_, updates_;
        vector<char> lazy_;
        int s_ = 1;
        T identity_element_{}, identity_update_{};
    };
"""

write("hld_lazy_segment_tree.cpp", f"""// @aliases HLD_lazy_segment_tree
// HLD + lazy segment tree: path add, path sum (change the nested lazy_sgt hooks for other operations).
//   hld_lazy_segment_tree<ll> h(root, n, adj, val);  h.update(a, b, x); h.query(a, b);  plus get_lca, ...
{NOTE}// Note: the constructor reorders adj (heavy child first).
template <class T>
class hld_lazy_segment_tree {{
public:
{MEMBERS}
    hld_lazy_segment_tree(int root, int n, vector<int> adj[], const vector<T> &arr, bool val_edges = false)
        : {INIT}, val_edges_(val_edges) {{
        first_.assign(n, 0);
        dfs(root, -1, adj);
        dfs_tour(root, false, adj);
        st_ = euler_sparse_table(euler, depth);
        seg_ = lazy_sgt(tour_list, arr);
    }}

{NAV}
    T query(int a, int b) {{  // sum over the path a - b
        T ans{{}};
        for (; heavy_root[a] != heavy_root[b]; a = parent[heavy_root[a]]) {{
            if (depth[heavy_root[a]] < depth[heavy_root[b]]) swap(a, b);
            ans = seg_.merge(ans, seg_.query(tour_start[heavy_root[a]], tour_start[a]));
        }}
        if (depth[a] > depth[b]) swap(a, b);
        ans = seg_.merge(ans, seg_.query(tour_start[a] + val_edges_, tour_start[b]));
        return ans;
    }}

    void update(int a, int b, T x) {{  // add x on the path a - b
        for (; heavy_root[a] != heavy_root[b]; a = parent[heavy_root[a]]) {{
            if (depth[heavy_root[a]] < depth[heavy_root[b]]) swap(a, b);
            seg_.update(tour_start[heavy_root[a]], tour_start[a], x);
        }}
        if (depth[a] > depth[b]) swap(a, b);
        seg_.update(tour_start[a] + val_edges_, tour_start[b], x);
    }}

private:
{EULER_SPARSE}
{LAZY}
{DFS}
    vector<int> first_;
    int tour_ = 0, timer_ = 0;
    bool val_edges_;
    euler_sparse_table st_;
    lazy_sgt seg_;
}};
""")
