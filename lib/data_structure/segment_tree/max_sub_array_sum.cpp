class max_subarray_seg_tree {
public:
    vector<array<int, 4>> tree; //sum, maxpref, maxsuf, maxseg
    int s = 1;
    array<int, 4> identity_element = {0, 0, 0, 0};

    max_subarray_seg_tree(vector<int> &a) {
        int n = size(a);
        while (s < n) s <<= 1;
        tree.resize(2 * s, identity_element);

        for (int i = 0; i < n; i++) {
            int maxi = max<int>(0, a[i]);
            tree[s + i] = {a[i], maxi, maxi, maxi};
        }

        for (int i = s - 1; i >= 1; i--)
            tree[i] = merge(tree[2 * i], tree[2 * i + 1]);
    }

    array<int, 4> query(int l, int r, int node, int lo, int hi) {
        if (l <= lo and hi <= r) return tree[node];
        else if (lo > r or hi < l) return identity_element;

        int mid = (lo + hi) >> 1;
        auto lhs = query(l, r, 2 * node, lo , mid);
        auto rhs = query(l, r, 2 * node + 1, mid + 1, hi);

        return merge(lhs, rhs);
    }

    void update(int i, int val) {
        int maxi = max<int>(0, val);
        tree[s + i] = {val, maxi, maxi, maxi};
        for (int j = (s + i) / 2; j >= 1; j /= 2) {
            tree[j] = merge(tree[2 * j], tree[2 * j + 1]);
        }
    }

    int query(int l, int r) {
        return query(l, r, 1, 0, s - 1)[3];
    }

    array<int, 4> merge(array<int, 4> lhs, array<int, 4> rhs) {
        array<int, 4> ans;
        ans[0] = lhs[0] + rhs[0];
        ans[1] = max(lhs[1], lhs[0] + rhs[1]);
        ans[2] = max(rhs[2], rhs[0] + lhs[2]);
        ans[3] = max({lhs[3], rhs[3], lhs[2] + rhs[1]});
        return ans;
    }
};
