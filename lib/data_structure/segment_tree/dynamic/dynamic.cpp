// @aliases Dynamic
// Dynamic (sparse) segment tree over indices [1, n], nodes created on demand. Point add, range sum (inclusive).
//   dynamic<ll> st(1e9); st.update(i, x); st.query(l, r);   // change the marked lines for other ops
template <class T>
class dynamic {
public:
    explicit dynamic(long long n) : n_(n), root_(new bird()) {}
    ~dynamic() { delete root_; }
    dynamic(const dynamic &) = delete;
    dynamic &operator=(const dynamic &) = delete;

    T query(long long l, long long r) const { return query(l, r, root_, 1, n_); }  // make sure to check ranges
    void update(long long index, T val) { update(index, val, root_, 1, n_); }

private:
    struct bird {
        T value{};
        bird *left_child = nullptr, *right_child = nullptr;
        ~bird() {
            delete left_child;
            delete right_child;
        }
    };

    T query(long long l, long long r, bird *node, long long lo, long long hi) const {
        if (l <= lo and hi <= r) return node->value;
        if (lo > r or hi < l) return identity_element_;

        long long mid = lo + (hi - lo) / 2;
        T lhs = identity_element_, rhs = identity_element_;
        if (node->left_child) lhs = query(l, r, node->left_child, lo, mid);
        if (node->right_child) rhs = query(l, r, node->right_child, mid + 1, hi);

        return lhs + rhs;  // remember to change
    }

    void update(long long index, T new_value, bird *node, long long lo, long long hi) {
        if (lo > index or hi < index) return;
        if (lo == hi) {
            node->value += new_value;  // remember to change
            return;
        }

        long long mid = lo + (hi - lo) / 2;
        if (index <= mid) {
            if (!node->left_child) node->left_child = new bird();
            update(index, new_value, node->left_child, lo, mid);
        } else {
            if (!node->right_child) node->right_child = new bird();
            update(index, new_value, node->right_child, mid + 1, hi);
        }

        T res = identity_element_;
        if (node->left_child) res += node->left_child->value;
        if (node->right_child) res += node->right_child->value;
        node->value = res;
    }

    long long n_;
    bird *root_;
    T identity_element_{};
};
