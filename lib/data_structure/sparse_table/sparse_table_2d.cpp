// @aliases SparseTable2D
// 2D range minimum, O(1) query.   sparse_table_2d<ll> st(grid);
//   st.query(i1, j1, x, y)  // min over the x-by-y rectangle whose upper-left corner is (i1, j1)
template <typename T>
class sparse_table_2d {
public:
    explicit sparse_table_2d(const vector<vector<T>> &v) : n_(v.size()), m_(v.front().size()) {
        lg_.resize(max(n_, m_) + 1);
        for (int i = 2; i <= max(n_, m_); i++) lg_[i] = lg_[i >> 1] + 1;

        table_.resize(lg_[n_] + 1, vector<vector<vector<T>>>(lg_[m_] + 1));
        table_[0][0] = v;

        for (int b = 1; b <= lg_[m_]; b++) {
            table_[0][b].resize(n_, vector<T>(m_ - (1 << b) + 1));
            for (int c = 0; c < n_; c++)
                for (int d = 0; d + (1 << b) - 1 < m_; d++)
                    table_[0][b][c][d] = min(table_[0][b - 1][c][d], table_[0][b - 1][c][d + (1 << (b - 1))]);
        }

        for (int a = 1; a <= lg_[n_]; a++) {
            table_[a][0].resize(n_ - (1 << a) + 1, vector<T>(m_));
            for (int c = 0; c + (1 << a) - 1 < n_; c++)
                for (int d = 0; d < m_; d++)
                    table_[a][0][c][d] = min(table_[a - 1][0][c][d], table_[a - 1][0][c + (1 << (a - 1))][d]);
        }

        for (int a = 1; a <= lg_[n_]; a++) {
            for (int b = 1; b <= lg_[m_]; b++) {
                table_[a][b].resize(n_ - (1 << a) + 1, vector<T>(m_ - (1 << b) + 1));
                for (int c = 0; c + (1 << a) - 1 < n_; c++) {
                    for (int d = 0; d + (1 << b) - 1 < m_; d++) {
                        T upper_lhs = table_[a - 1][b - 1][c][d];
                        T upper_rhs = table_[a - 1][b - 1][c][d + (1 << (b - 1))];
                        T bottom_lhs = table_[a - 1][b - 1][c + (1 << (a - 1))][d];
                        T bottom_rhs = table_[a - 1][b - 1][c + (1 << (a - 1))][d + (1 << (b - 1))];
                        table_[a][b][c][d] = min({upper_lhs, upper_rhs, bottom_lhs, bottom_rhs});
                    }
                }
            }
        }
    }

    T query(int i1, int j1, int x, int y) const {  // upper corner, rows, cols
        int lg1 = lg_[x], lg2 = lg_[y];
        int i2 = i1 + x - (1 << lg1);
        int j2 = j1 + y - (1 << lg2);

        assert(i1 >= 0 and i1 + x <= n_);
        assert(j1 >= 0 and j1 + y <= m_);

        const auto &t = table_[lg1][lg2];
        return min({t[i1][j1], t[i1][j2], t[i2][j1], t[i2][j2]});
    }

private:
    int n_, m_;
    vector<int> lg_;
    vector<vector<vector<vector<T>>>> table_;
};
