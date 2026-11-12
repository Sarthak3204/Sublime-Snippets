// @needs dsu_rollback
class Dynamic_Connectivity {
public:
    dsu_rollback d;
    vector<int> ans;
    vector<vector<tuple<int, int, int>>> tree; //a, b, united
    int n, s = 1;

    Dynamic_Connectivity(int q, int n) : n(n) {
        while (s < q) s <<= 1;
        ans.resize(s);
        tree.resize(s << 1);
        d = dsu_rollback(n);
    }

    void update(int a, int b, int l, int r, int node, int lo, int hi) {
        if (l <= lo and hi <= r) {
            tree[node].push_back(make_tuple(a, b, 0));
            return;
        }
        if (lo > r or hi < l) return;

        int mid = (lo + hi) >> 1;
        update(a, b, l, r, node << 1, lo, mid);
        update(a, b, l, r, node << 1 | 1, mid + 1, hi);
    }

    void update(int a, int b, int l, int r) {//[l, r] -> active
        if (l > r) return;
        update(a, b, l, r, 1, 0, s - 1);
    }

    void dfs(int node, int lo, int hi) {
        for (auto &[a, b, united] : tree[node])
            united = d.unite(a, b);

        if (lo != hi) {
            int mid = (lo + hi) >> 1;
            dfs(node << 1, lo, mid);
            dfs(node << 1 | 1, mid + 1, hi);
        }
        else ans[lo] = d.components;

        for (auto &[a, b, united] : tree[node])
            if (united) d.rollback();
    }

    void dfs() {
        dfs(1, 0, s - 1);
    }
};
