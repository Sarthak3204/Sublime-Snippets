// @aliases MinSparseTable
// Range minimum, O(1) query on inclusive [l, r].   min_sparse_table<ll> st(a); st.query(l, r);
template <typename T>
class min_sparse_table {
public:
    explicit min_sparse_table(const vector<T> &a) : n_(a.size()), lg_(n_ + 1) {
        for (int i = 2; i <= n_; i++) lg_[i] = lg_[i >> 1] + 1;

        table_.resize(lg_[n_] + 1);
        table_[0] = a;
        for (int j = 1; j <= lg_[n_]; j++) {
            table_[j].resize(n_ - (1 << j) + 1);
            for (int i = 0; i + (1 << j) - 1 < n_; i++)
                table_[j][i] = min(table_[j - 1][i], table_[j - 1][i + (1 << (j - 1))]);
        }
    }

    T query(int l, int r) const {
        int j = lg_[r - l + 1];
        return min(table_[j][l], table_[j][r - (1 << j) + 1]);
    }

private:
    int n_;
    vector<int> lg_;
    vector<vector<T>> table_;
};
