class first_atleast_seg_tree {
public:
    vector<int> tree, b;  // b: own copy of the values (the original resized the caller's vector)
    int s = 1, identity_element = INT_MIN;

    first_atleast_seg_tree(const vector<int> &a) : b(a) {
        int n = static_cast<int>(a.size());
        while (s < n) s *= 2;

        tree.resize(2 * s), b.resize(2 * s, identity_element);

        iota(tree.begin() + s, tree.end(), 0);

        for (int i = s - 1; i >= 1; i--)
            tree[i] = merge(tree[2 * i], tree[2 * i + 1]);
    }

    int query(int x, int l, int node, int lo, int hi) {
        if (b[tree[node]] < x) return -1;
        else if (lo == hi) return lo;
        int mid = (lo + hi) / 2, index;
        if (mid >= l) {
            index = query(x, l, 2 * node, lo, mid);
            if (index != -1) return index;
        }
        index = query(x, l, 2 * node + 1, mid + 1, hi);
        return index;
    }

    void update(int i, int val) {
        b[i] = val;
        for (int j = (s + i) / 2; j >= 1; j /= 2)
            tree[j] = merge(tree[2 * j], tree[2 * j + 1]);
    }
    int query(int x, int l) {return query(x, l, 1, 0, s - 1);}//j >= l and a[j] >= x
    int merge(int lhs, int rhs) {return (max(b[lhs], b[rhs]) == b[lhs] ? lhs : rhs);}
};
