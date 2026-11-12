#include "test_prelude.hpp"

namespace pf {
#include "lib/string/prefix_function.cpp"
#include "lib/string/kmp.cpp"  // @needs prefix_function
}
namespace man {
#include "lib/string/manacher.cpp"
}
namespace lcp_ns {
#include "lib/string/lcp.cpp"
}
namespace pal {
#include "lib/string/is_palindrome.cpp"
}
namespace spal {
#include "lib/string/prefix_function.cpp"  // @needs
#include "lib/string/shortest_palindrome.cpp"
}
namespace lexo {
#include "lib/string/lexo_substring.cpp"
}
namespace sa {
#include "lib/string/suffix_automation.cpp"
}
namespace ts {
#include "lib/string/trie/trie_string.cpp"
}
namespace tx {
#include "lib/string/trie/trie_max_xor.cpp"
}
namespace sh {
#include "lib/string/string_hashing.cpp"
vector<int> rabin_karp(string s, string t) {  // statement-level snippet: needs s, t, n, m
    int n = s.size(), m = t.size();
#include "lib/string/rabin_karp.cpp"
    RabinKarp();
    return occurences;
}
}  // namespace sh
namespace ph {
#include "lib/string/poly_hash.cpp"
}

string random_str(tu::Gen &g, int n, char hi = 'c') {
    string s(n, 'a');
    for (auto &c : s) c = g.range('a', hi);
    return s;
}
bool brute_pal(const string &s) { return s == string(s.rbegin(), s.rend()); }
const bool hashes_ready = (sh::precompute(), ph::precompute(), true);

TEST(Strings, PrefixFunctionKmpAndRabinKarp) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            string s = random_str(g, n, 'b');
            auto pi = pf::prefix_function(s);
            for (int i = 0; i < n; i++) {
                int want = 0;
                for (int k = 1; k <= i; k++)
                    if (s.substr(0, k) == s.substr(i - k + 1, k)) want = k;
                ASSERT_EQ(pi[i], want) << s << " " << i;
            }
            string t = random_str(g, g.range(1, 4), 'b');
            vector<int> occ;
            for (int i = 0; i + (int)t.size() <= n; i++)
                if (s.compare(i, t.size(), t) == 0) occ.push_back(i);
            ASSERT_EQ(pf::kmp(s, t), occ) << s << " " << t;
            ASSERT_EQ(sh::rabin_karp(s, t), occ) << s << " " << t;
        }
    }
}

TEST(Strings, ManacherMatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            string s = random_str(g, n, 'b'), copy = s;
            man::Manacher m(s);
            int best = 0;
            for (int l = 0; l < n; l++)
                for (int r = l; r < n; r++) {
                    bool p = brute_pal(s.substr(l, r - l + 1));
                    ASSERT_EQ(m.isPalindrome(l, r), p) << s << " " << l << " " << r;
                    if (p) best = max<int>(best, r - l + 1);
                }
            for (int c = 0; c < n; c++) {
                int odd = 1, even = 0;
                while (c - odd >= 0 and c + odd < n and s[c - odd] == s[c + odd]) odd++;
                while (c - even >= 0 and c + 1 + even < n and s[c - even] == s[c + 1 + even]) even++;
                ASSERT_EQ(m.Longest(c, 1), 2 * odd - 1) << s << " " << c;
                ASSERT_EQ(m.Longest(c, 0), 2 * even) << s << " " << c;
            }
            string lp = m.getLongest();
            ASSERT_EQ((int)lp.size(), best) << s;
            ASSERT_TRUE(brute_pal(lp) and s.find(lp) != string::npos) << s << " " << lp;
            ASSERT_EQ(s, copy);
        }
    }
}

TEST(Strings, LcpLexoSubstringAndSuffixArray) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            if (n > 31) continue;
            string s = random_str(g, n, 'b');
            auto lcp = lcp_ns::LCP(n, s);
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) {
                    int k = 0;
                    while (i + k < n and j + k < n and s[i + k] == s[j + k]) k++;
                    ASSERT_EQ(lcp[i][j], k);
                }

            lexo::Lexo_substring ls(n, s);
            vector<pair<int, int>> want;
            for (int i = 0; i < n; i++)
                for (int j = i; j < n; j++) want.push_back({i, j});
            sort(want.begin(), want.end(), [&](auto a, auto b) {  // by text; equal texts: later start first
                auto x = s.substr(a.first, a.second - a.first + 1), y = s.substr(b.first, b.second - b.first + 1);
                return x != y ? x < y : a.first > b.first;
            });
            ASSERT_EQ(ls.substrings, want) << s;

            string t = s;
            auto order = sa::suffix_array(t);  // appends '$' to its argument
            ASSERT_EQ(t, s + "$");
            vector<int> want_sa(n + 1);
            iota(want_sa.begin(), want_sa.end(), 0);
            sort(want_sa.begin(), want_sa.end(), [&](int a, int b) { return t.substr(a) < t.substr(b); });
            ASSERT_EQ(order, want_sa) << s;
        }
    }
}

TEST(Strings, PalindromeHelpers) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            string s = random_str(g, n, 'b');
            ASSERT_EQ(pal::is_Palindrome(s), brute_pal(s));
            string want;  // shortest palindrome by adding characters in front
            for (int k = n; k >= 0; k--)
                if (brute_pal(s.substr(0, k))) {
                    string add = s.substr(k);
                    want = string(add.rbegin(), add.rend()) + s;
                    break;
                }
            ASSERT_EQ(spal::shortestPalindrome(s), want) << s;
        }
    }
    string e;
    EXPECT_TRUE(pal::is_Palindrome(e));
    EXPECT_EQ(spal::shortestPalindrome(""), "");
}

TEST(Strings, HashesDistinguishSubstrings) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            string s = random_str(g, n, 'b');
            sh::HashedString h(s);
            ph::PolyHash p(s);
            for (int a = 0; a < n; a++)
                for (int b = a; b < n; b++)
                    for (int c = 0; c + (b - a) < n; c++) {
                        int d = c + (b - a);
                        bool same = s.compare(a, b - a + 1, s, c, d - c + 1) == 0;
                        ASSERT_EQ(h.getHash(a, b) == h.getHash(c, d), same) << s;
                        ASSERT_EQ(p.getF(a, b) == p.getF(c, d), same) << s;
                        ASSERT_EQ(p.getB(a, b) == p.getB(c, d), same) << s;
                    }
            for (int l = 0; l < n; l++)  // palindrome check: forward hash == backward hash
                for (int r = l; r < n; r++) ASSERT_EQ(p.getF(l, r) == p.getB(l, r), brute_pal(s.substr(l, r - l + 1))) << s;
        }
    }
}

// Same behaviour as the generic trie (trie_vector.cpp), on lowercase strings.
TEST(Strings, TrieStringMatchesMultiset) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        ts::trie t;
        multiset<string> words;
        for (int op = 0; op < 300; op++) {
            string s = random_str(g, g.range(0, 4), 'c');
            int kind = g.range(0, 6);
            if (kind <= 1) t.insert(s), words.insert(s);
            else if (kind == 2) {
                bool all = g.coin();
                t.erase(s, all);
                if (words.count(s)) all ? (void)words.erase(s) : (void)words.erase(words.find(s));
            } else {
                int exact = words.count(s), with_prefix = 0, prefix_words = 0, longest = 0;
                for (auto &w : words) {
                    with_prefix += w.compare(0, s.size(), s) == 0;
                    prefix_words += s.compare(0, w.size(), w) == 0;
                    int k = 0;
                    while (k < (int)min(w.size(), s.size()) and w[k] == s[k]) k++;
                    longest = max(longest, k);
                }
                ASSERT_EQ(t.find(s), exact) << "seed=" << seed << " op=" << op << " " << s;
                ASSERT_EQ(t.count(s), exact);
                ASSERT_EQ(t.count(s, true), with_prefix) << "seed=" << seed << " op=" << op << " '" << s << "'";
                ASSERT_EQ(t.countPrefixWord(s), prefix_words) << "seed=" << seed << " op=" << op << " " << s;
                ASSERT_EQ(t.maxPrefixWord(s), longest) << "seed=" << seed << " op=" << op << " " << s;
                ASSERT_EQ(t.wordCount, (int)words.size());
            }
        }
    }
}

TEST(Strings, TrieMaxXorMatchesBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        tx::trie t;
        vector<ll> vals;
        for (int op = 0; op < 200; op++) {
            ll x = g.coin() ? g.range(0, 15) : g.range(0, (1 << 30) - 1);
            if (vals.empty() or g.coin()) t.insert(x), vals.push_back(x);
            else {
                ll want = 0;
                for (ll v : vals) want = max(want, v ^ x);
                ASSERT_EQ(t.calc(x), want) << "seed=" << seed;
            }
        }
    }
}
