// @aliases SegTree
// Segment tree (recursive, 0-indexed, inclusive [l, r]). Generic over the value type and the merge.
//   seg_tree st(a, 0LL, [](ll x, ll y) { return x + y; });   // values, identity element, merge
//   st.update(l, r, val);   // applies `apply` to every position in [l, r] (point update: l == r)
//   st.query(l, r);
template <class T, class Merge>
class seg_tree {
public:
    seg_tree(int n, T identity_element, Merge merge = Merge()) : identity_element_(identity_element), merge_(merge) {
        while (s_ < n) s_ <<= 1;
        tree_.assign(s_ << 1, identity_element_);
    }
    seg_tree(const vector<T> &a, T identity_element, Merge merge = Merge()) : seg_tree(a.size(), identity_element, merge) {
        for (int i = 0; i < (int)a.size(); i++) tree_[s_ + i] = a[i];
        for (int i = s_ - 1; i >= 1; i--) tree_[i] = merge_(tree_[i << 1], tree_[i << 1 | 1]);
    }

    T query(int l, int r) const { return query(l, r, 1, 0, s_ - 1); }
    void update(int l, int r, T val) { update(l, r, val, 1, 0, s_ - 1); }

private:
    T query(int l, int r, int node, int lo, int hi) const {
        if (l <= lo and hi <= r) return tree_[node];
        if (lo > r or hi < l) return identity_element_;

        int mid = (lo + hi) >> 1;
        auto lhs = query(l, r, node << 1, lo, mid);
        auto rhs = query(l, r, node << 1 | 1, mid + 1, hi);

        return merge_(lhs, rhs);
    }

    void update(int l, int r, T val, int node, int lo, int hi) {
        if (lo > r or hi < l) return;
        if (lo == hi) {
            apply(node, val);
            return;
        }

        int mid = (lo + hi) >> 1;
        update(l, r, val, node << 1, lo, mid);
        update(l, r, val, node << 1 | 1, mid + 1, hi);

        tree_[node] = merge_(tree_[node << 1], tree_[node << 1 | 1]);
    }

    void apply(int node, T val) {  // remember to change: assignment by default
        tree_[node] = val;
    }

    vector<T> tree_;
    int s_ = 1;
    T identity_element_;
    Merge merge_;
};
