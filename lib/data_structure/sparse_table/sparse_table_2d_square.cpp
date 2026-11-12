// @aliases SparseTable2D_square
// 2D range minimum over squares, O(1) query.   sparse_table_2d_square<ll> st(grid);
//   st.query(i1, j1, x)  // min over the x-by-x square whose upper-left corner is (i1, j1)
template <typename T>
class sparse_table_2d_square {
public:
    explicit sparse_table_2d_square(const vector<vector<T>> &v) : n_(v.size()), m_(v.front().size()) {
        lg_.resize(min(n_, m_) + 1);
        for (int i = 2; i <= min(n_, m_); i++) lg_[i] = lg_[i >> 1] + 1;

        table_.resize(lg_[min(n_, m_)] + 1);
        table_[0] = v;

        for (int a = 1; a <= lg_[min(n_, m_)]; a++) {
            table_[a].resize(n_ - (1 << a) + 1, vector<T>(m_ - (1 << a) + 1));
            for (int c = 0; c + (1 << a) - 1 < n_; c++) {
                for (int d = 0; d + (1 << a) - 1 < m_; d++) {
                    T upper_lhs = table_[a - 1][c][d];
                    T upper_rhs = table_[a - 1][c][d + (1 << (a - 1))];
                    T bottom_lhs = table_[a - 1][c + (1 << (a - 1))][d];
                    T bottom_rhs = table_[a - 1][c + (1 << (a - 1))][d + (1 << (a - 1))];
                    table_[a][c][d] = min({upper_lhs, upper_rhs, bottom_lhs, bottom_rhs});
                }
            }
        }
    }

    T query(int i1, int j1, int x) const {
        int i2 = i1 + x - (1 << lg_[x]);
        int j2 = j1 + x - (1 << lg_[x]);

        assert(i1 >= 0 and i1 + x <= n_);
        assert(j1 >= 0 and j1 + x <= m_);

        const auto &t = table_[lg_[x]];
        return min({t[i1][j1], t[i1][j2], t[i2][j1], t[i2][j2]});
    }

private:
    int n_, m_;
    vector<int> lg_;
    vector<vector<vector<T>>> table_;
};
