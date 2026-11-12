// @aliases SparseTable
// Sparse table for idempotent ops (min, max, gcd, &, |). O(n log n) build, O(1) query on inclusive [l, r].
//   sparse_table st(a, [](ll x, ll y) { return min(x, y); });  st.query(l, r);
template <typename T, class F>
class sparse_table {
public:
    sparse_table(const vector<T> &a, const F &f) : n_(a.size()), lg_(n_ + 1), func_(f) {
        for (int i = 2; i <= n_; i++) lg_[i] = lg_[i >> 1] + 1;

        table_.resize(lg_[n_] + 1);
        table_[0] = a;
        for (int j = 1; j <= lg_[n_]; j++) {
            table_[j].resize(n_ - (1 << j) + 1);
            for (int i = 0; i + (1 << j) - 1 < n_; i++)
                table_[j][i] = func_(table_[j - 1][i], table_[j - 1][i + (1 << (j - 1))]);
        }
    }

    T query(int l, int r) const {
        int j = lg_[r - l + 1];
        return func_(table_[j][l], table_[j][r - (1 << j) + 1]);
    }

private:
    int n_;
    vector<int> lg_;
    vector<vector<T>> table_;
    F func_;
};
