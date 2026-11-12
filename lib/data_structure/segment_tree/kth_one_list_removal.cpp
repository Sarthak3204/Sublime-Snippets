class kth_one_seg_tree {
public:
    vector<int> tree;
    int s = 1, identity_element = 0;

    kth_one_seg_tree(int n) {
        while (s < n) s *= 2;
        tree.resize(2 * s, identity_element);

        fill(tree.begin() + s, tree.begin() + s + n, 1);

        for (int i = s - 1; i >= 1; i--)
            tree[i] = merge(tree[2 * i], tree[2 * i + 1]);
    }

    int query(int k, int node, int lo, int hi) {
        if (lo == hi) return lo;
        int mid = (lo + hi) / 2;
        if (tree[2 * node] < k) return query(k - tree[2 * node], 2 * node + 1, mid + 1, hi);
        else return query(k, 2 * node, lo, mid);
    }

    void update(int i, int val) {
        tree[s + i] = val;
        for (int j = (s + i) / 2; j >= 1; j /= 2) {
            tree[j] = merge(tree[2 * j], tree[2 * j + 1]);
        }
    }

    int query(int k) {return query(k, 1, 0, s - 1);}
    int merge(int lhs, int rhs) {return lhs + rhs;}
};
