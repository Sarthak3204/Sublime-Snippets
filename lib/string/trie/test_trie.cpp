#include "test_prelude.hpp"

namespace impl {
#include "lib/string/trie/trie_vector.cpp"
}

// Trie over digit sequences, compared with a multiset of words.
TEST(Trie, InsertEraseCountsMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        impl::trie<10> t;
        multiset<vector<ll>> words;
        auto rand_word = [&] { return g.vec(g.range(1, 4), 0, 2); };  // tiny alphabet: lots of sharing
        for (int op = 0; op < 300; op++) {
            vector<ll> w = rand_word();
            switch (g.range(0, 2)) {
            case 0:
                t.insert(w), words.insert(w);
                break;
            case 1: {
                bool all_occurrence = g.coin();
                t.erase(w, all_occurrence);
                if (all_occurrence) words.erase(w);
                else if (words.count(w)) words.erase(words.find(w));
                break;
            }
            default: {
                ll exact = words.count(w), with_prefix = 0, prefixes_of_w = 0, lcp = 0;
                for (const auto &x : words) {
                    with_prefix += x.size() >= w.size() && equal(w.begin(), w.end(), x.begin());
                    prefixes_of_w += x.size() <= w.size() && equal(x.begin(), x.end(), w.begin());
                    ll k = mismatch(w.begin(), w.end(), x.begin(), x.end()).first - w.begin();
                    lcp = max(lcp, k);
                }
                ASSERT_EQ(t.find(w), exact) << seed << " op " << op;
                ASSERT_EQ(t.count(w), exact);
                ASSERT_EQ(t.count(w, true), with_prefix);
                ASSERT_EQ(t.count_prefix_word(w), prefixes_of_w);
                ASSERT_EQ(t.max_prefix_word(w), lcp) << seed << " op " << op;  // was wrong after erase
                ASSERT_EQ(t.word_count, (ll)words.size());
            }
            }
        }
    }
}
