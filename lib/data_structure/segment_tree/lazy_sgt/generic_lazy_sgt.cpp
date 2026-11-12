// @aliases generic_LazySGT
// Generic lazy segment tree (recursive, 0-indexed, inclusive [l, r]) over a value type T and an update type U:
//   T()          identity element        T + T   merge
//   U()          identity update         T += U  apply an update to a node value
//   U += U       compose: `a += b` means "a, then b"
// Example below: range add, range sum (the value keeps its segment length).
struct node_value {
    ll sum = 0, len = 0;  // T() = identity element
    node_value operator+(const node_value &o) const { return {sum + o.sum, len + o.len}; }
};
struct node_update {
    ll add = 0;  // U() = identity update
    void operator+=(const node_update &o) { add += o.add; }
};
inline void operator+=(node_value &v, const node_update &u) { v.sum += u.add * v.len; }

template <class T, class U>
class generic_lazy_sgt {
public:
    explicit generic_lazy_sgt(int n) {
        while (s_ < n) s_ <<= 1;
        tree_.assign(s_ << 1, T());
        updates_.assign(s_ << 1, U());
    }
    explicit generic_lazy_sgt(const vector<T> &a) : generic_lazy_sgt(a.size()) {
        for (int i = 0; i < (int)a.size(); i++) tree_[s_ + i] = a[i];
        for (int i = s_ - 1; i >= 1; i--) tree_[i] = tree_[i << 1] + tree_[i << 1 | 1];
    }

    T query(int l, int r) { return query(l, r, 1, 0, s_ - 1); }
    void update(int l, int r, const U &upd) { update(l, r, 1, 0, s_ - 1, upd); }

private:
    void pushdown(int node, int lo, int hi) {
        tree_[node] += updates_[node];
        if (lo != hi) {
            updates_[node << 1] += updates_[node];
            updates_[node << 1 | 1] += updates_[node];
        }
        updates_[node] = U();
    }

    T query(int l, int r, int node, int lo, int hi) {
        pushdown(node, lo, hi);
        if (l <= lo and hi <= r) return tree_[node];
        if (lo > r or hi < l) return T();

        int mid = (lo + hi) >> 1;
        auto lhs = query(l, r, node << 1, lo, mid);
        auto rhs = query(l, r, node << 1 | 1, mid + 1, hi);

        return lhs + rhs;
    }

    void update(int l, int r, int node, int lo, int hi, const U &upd) {
        pushdown(node, lo, hi);
        if (l <= lo and hi <= r) {
            updates_[node] += upd;
            pushdown(node, lo, hi);
            return;
        }
        if (lo > r or hi < l) return;

        int mid = (lo + hi) >> 1;
        update(l, r, node << 1, lo, mid, upd);
        update(l, r, node << 1 | 1, mid + 1, hi, upd);

        tree_[node] = tree_[node << 1] + tree_[node << 1 | 1];
    }

    vector<T> tree_;
    vector<U> updates_;
    int s_ = 1;
};
