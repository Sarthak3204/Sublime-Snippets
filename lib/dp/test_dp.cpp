#include "test_prelude.hpp"

namespace num_lis {
#include "lib/dp/lis/find_number_of_lis.cpp"
}
namespace lcs_ns {
#include "lib/dp/lcs/lcs.cpp"
#include "lib/dp/lcs/print_lcs.cpp"
}
namespace lis_n2 {
#include "lib/dp/lis/lis.cpp"
}
namespace lis_inc {
#include "lib/dp/lis/lis_increasing.cpp"
}
namespace lis_nd {
#include "lib/dp/lis/lis_non_decreasing.cpp"
}

bool is_subsequence(const vector<ll> &sub, const vector<ll> &v) {
    size_t k = 0;
    for (ll x : v)
        if (k < sub.size() and sub[k] == x) k++;
    return k == sub.size();
}

TEST(Dp, LisVariantsMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : {0, 1, 2, 3, 4, 5, 7, 8, 9, 12}) {
            auto v = g.vec(n, -3, 3);
            int best_inc = 0, best_nd = 0, count_inc = 0;
            for (int mask = 0; mask < (1 << n); mask++) {  // every subsequence (by index set)
                vector<ll> s;
                for (int i = 0; i < n; i++)
                    if (mask >> i & 1) s.push_back(v[i]);
                bool inc = true, nd = true;
                for (int i = 1; i < (int)s.size(); i++) inc &= s[i - 1] < s[i], nd &= s[i - 1] <= s[i];
                int len = s.size();
                if (inc) {
                    if (len > best_inc) best_inc = len, count_inc = 0;
                    if (len == best_inc) count_inc++;
                }
                if (nd) best_nd = max(best_nd, len);
            }
            ASSERT_EQ(lis_n2::LIS(v, n), best_inc) << "seed=" << seed << " n=" << n;
            if (n > 0) {
                ASSERT_EQ(num_lis::findNumberOfLIS(v), count_inc) << "seed=" << seed << " n=" << n;
            }

            auto a = lis_inc::lis(n, v);
            ASSERT_EQ((int)a.size(), best_inc);
            ASSERT_TRUE(is_subsequence(a, v) and is_sorted(a.begin(), a.end()) and adjacent_find(a.begin(), a.end()) == a.end());
            auto b = lis_nd::lis(n, v);
            ASSERT_EQ((int)b.size(), best_nd);
            ASSERT_TRUE(is_subsequence(b, v) and is_sorted(b.begin(), b.end()));
        }
    }
}

TEST(Dp, LcsAndPrintLcsMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : {0, 1, 2, 5, 8}) {
            for (int m : {0, 1, 3, 7}) {
                string s(n, 'a'), t(m, 'a');
                for (auto &c : s) c = g.range('a', 'c');
                for (auto &c : t) c = g.range('a', 'c');
                int want = 0;  // longest common subsequence: every subsequence of s, checked in t
                for (int mask = 0; mask < (1 << n); mask++) {
                    string sub;
                    for (int i = 0; i < n; i++)
                        if (mask >> i & 1) sub += s[i];
                    size_t k = 0;
                    for (char c : t)
                        if (k < sub.size() and sub[k] == c) k++;
                    if (k == sub.size()) want = max<int>(want, sub.size());
                }
                vector dp(n + 1, vector<ll>(m + 1));
                ASSERT_EQ(lcs_ns::lcs(s, t, dp), want) << s << " " << t;

                ostringstream os;
                auto *old = cout.rdbuf(os.rdbuf());
                lcs_ns::print_lcs(s, t, dp);
                cout.rdbuf(old);
                string p = os.str();
                ASSERT_EQ((int)p.size(), want) << s << " " << t;
                auto subseq = [&](const string &x) {
                    size_t k = 0;
                    for (char c : x)
                        if (k < p.size() and p[k] == c) k++;
                    return k == p.size();
                };
                ASSERT_TRUE(subseq(s) and subseq(t)) << s << " " << t << " -> " << p;
            }
        }
    }
}
