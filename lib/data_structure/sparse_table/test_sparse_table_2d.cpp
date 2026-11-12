#include "test_prelude.hpp"

namespace rect {
#include "lib/data_structure/sparse_table/sparse_table_2d.cpp"
}
namespace square {
#include "lib/data_structure/sparse_table/sparse_table_2d_square.cpp"
}

ll brute_min(const vector<vector<ll>> &a, int i, int j, int x, int y) {
    ll res = LLONG_MAX;
    for (int r = i; r < i + x; r++)
        for (int c = j; c < j + y; c++) res = min(res, a[r][c]);
    return res;
}

TEST(SparseTable2D, RectangleAndSquareMinMatchBruteForce) {
    const vector<int> dims = {1, 2, 3, 5, 8, 9};
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : dims)
            for (int m : dims) {
                vector<vector<ll>> a(n);
                for (auto &row : a) row = g.vec(m, -100, 100);
                rect::sparse_table_2d<ll> t(a);
                square::sparse_table_2d_square<ll> sq(a);
                for (int q = 0; q < 30; q++) {
                    int x = g.range(1, n), y = g.range(1, m);  // height, width
                    int i = g.range(0, n - x), j = g.range(0, m - y);
                    ASSERT_EQ(t.query(i, j, x, y), brute_min(a, i, j, x, y)) << n << "x" << m;
                    int s = g.range(1, min(n, m));
                    i = g.range(0, n - s), j = g.range(0, m - s);
                    ASSERT_EQ(sq.query(i, j, s), brute_min(a, i, j, s, s)) << n << "x" << m;
                }
            }
    }
}
