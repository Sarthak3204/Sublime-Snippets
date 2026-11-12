// @aliases LazySGT
// Lazy segment tree (recursive, 0-indexed, inclusive [l, r]). Range add + range sum by default:
// change merge / combine / apply (and the identities) for other operations.
//   lazy_sgt<ll> st(a);  st.update(l, r, x);  st.query(l, r);
template <class T>
class lazy_sgt {
public:
    explicit lazy_sgt(int n) {
        while (s_ < n) s_ <<= 1;
        tree_.assign(s_ << 1, identity_element_);
        lazy_.assign(s_ << 1, 0);
        updates_.assign(s_ << 1, identity_update_);
    }
    explicit lazy_sgt(const vector<T> &a) : lazy_sgt(a.size()) {
        for (int i = 0; i < (int)a.size(); i++) tree_[s_ + i] = a[i];
        for (int i = s_ - 1; i >= 1; i--) tree_[i] = merge(tree_[i << 1], tree_[i << 1 | 1]);
    }

    T query(int l, int r) { return query(l, r, 1, 0, s_ - 1); }
    void update(int l, int r, T upd) { update(l, r, 1, 0, s_ - 1, upd); }

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
        auto lhs = query(l, r, node << 1, lo, mid);
        auto rhs = query(l, r, node << 1 | 1, mid + 1, hi);

        return merge(lhs, rhs);
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

    T merge(T lhs, T rhs) { return lhs + rhs; }                                   // remember to change
    void combine(int node, int lo, int hi, T upd) { tree_[node] += (hi - lo + 1) * upd; }  // update on a whole node
    void apply(int node, T upd) {                                                 // compose pending updates
        lazy_[node] = 1;
        updates_[node] += upd;
    }

    vector<T> tree_, updates_;
    vector<char> lazy_;
    int s_ = 1;
    T identity_element_{}, identity_update_{};
};
