class max_prefix_seg_tree {
public:
    vector<pair<int, int>> tree;//sum, maxi
    int s = 1;
    pair<int, int> identity_element = {0, 0};

    max_prefix_seg_tree(vector<int> &a) {
        int n = static_cast<int>(a.size());
        while (s < n) s *= 2;
        tree.resize(2 * s, identity_element);

        for (int i = 0; i < n; i++)
            tree[s + i] = {a[i], max<int>(0, a[i])};

        for (int i = s - 1; i >= 1; i--)
            tree[i] = merge(tree[2 * i], tree[2 * i + 1]);
    }

    pair<int, int> query(int l, int r, int node, int lo, int hi) {
        if (l <= lo and hi <= r) return tree[node];
        else if (lo > r or hi < l) return identity_element;

        int mid = (lo + hi) / 2;
        auto lhs = query(l, r, 2 * node, lo , mid);
        auto rhs = query(l, r, 2 * node + 1, mid + 1, hi);

        return merge(lhs, rhs);
    }

    void update(int l, int r, int val, int node, int lo, int hi) {
        if (l <= lo and hi <= r) {
            tree[node] = {val, max<int>(0, val)}; //remember to change this
            return;
        }
        else if (lo > r or hi < l) return;

        int mid = (lo + hi) / 2;
        update(l, r, val, 2 * node, lo , mid);
        update(l, r, val, 2 * node + 1, mid + 1, hi);

        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    pair<int, int> query(int l, int r) {
        return query(l, r, 1, 0, s - 1);
    }

    void update(int l, int r, int val) {
        update(l, r, val, 1, 0, s - 1);
    }

    pair<int, int> merge(pair<int, int> lhs, pair<int, int> rhs) {
        int l = lhs.first + rhs.first;
        int r = max<int>({0, lhs.second, lhs.first + rhs.second});
        return {l, r};
    }
};
