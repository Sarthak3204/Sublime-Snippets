#include "test_prelude.hpp"

namespace impl {
#include "lib/miscellaneous/binary_to_decimal.cpp"
#include "lib/miscellaneous/decimal_to_binary.cpp"
#include "lib/miscellaneous/classcomp.cpp"
#include "lib/miscellaneous/compress_array.cpp"
#include "lib/miscellaneous/custom_hash.cpp"
#include "lib/miscellaneous/find_sqrt.cpp"
#include "lib/miscellaneous/hilbert_order.cpp"
#include "lib/miscellaneous/int128.cpp"
#include "lib/miscellaneous/kadane.cpp"
#include "lib/miscellaneous/merge_map.cpp"
#include "lib/miscellaneous/directions.cpp"
}  // namespace impl
#include "lib/miscellaneous/gp_hash_table.cpp"  // pb_ds include + using namespace: global, like in a solution
namespace dq_max {
#include "lib/miscellaneous/monotone_deque_max.cpp"
}
namespace dq_min {
#include "lib/miscellaneous/monotone_deque_min.cpp"
}

// namespace_io reads `stdin` and writes `stdout`: point both at temporary files while it is compiled.
FILE *io_in = nullptr, *io_out = nullptr;
#pragma push_macro("stdin")
#pragma push_macro("stdout")
#undef stdin
#undef stdout
#define stdin io_in
#define stdout io_out
#include "lib/miscellaneous/namespace_io.cpp"
#pragma pop_macro("stdin")
#pragma pop_macro("stdout")

// ---- statement-level snippets ----
ll mex_of(vector<ll> v) {
#include "lib/miscellaneous/calculate_mex.cpp"
    return calc(v);
}
pair<vector<ll>, vector<ll>> nge_nse(int n, const vector<ll> &v) {
#include "lib/miscellaneous/nge.cpp"
    {
#include "lib/miscellaneous/nse.cpp"
        return {NGE, NSE};
    }
}
vector<ll> compressed_coords(vector<ll> coord, const vector<ll> &queries) {
    using impl::custom_hash;
#include "lib/miscellaneous/coordinate_compression.cpp"
    vector<ll> r;
    for (ll q : queries) r.push_back(mp[q]);
    return r;
}

string i128_str(__int128 v) {
    ostringstream os;
    impl::operator<<(os, v);
    return os.str();
}

TEST(Misc, ConversionsAndSmallHelpers) {
    tu::Gen g(1);
    for (int it = 0; it < 5000; it++) {
        ll x = g.coin() ? g.range(0, 1000) : g.range(0, INF);
        string b = x ? "" : "0";
        for (ll y = x; y; y >>= 1) b += char('0' + (y & 1));
        reverse(b.begin(), b.end());
        ASSERT_EQ(impl::decimalToBinary(x), b);
        ASSERT_EQ(impl::binaryToDecimal(b), x);
        ll r = sqrtl((long double)x);
        while (r * r > x) r--;
        while ((r + 1) * (r + 1) <= x) r++;
        ASSERT_EQ(impl::findSqrt(x), r) << x;

        __int128 big = (__int128)g.range(-INF, INF) * g.range(-INF, INF);
        string want;
        __int128 t = big;
        bool neg = t < 0;
        do want += char('0' + (int)(neg ? -(t % 10) : t % 10)), t /= 10;
        while (t);
        if (neg) want += '-';
        ASSERT_EQ(i128_str(big), string(want.rbegin(), want.rend()));
    }
    EXPECT_EQ(impl::dir.size(), 8u);
}

TEST(Misc, ArrayHelpersMatchBruteForce) {
    for (int seed = 0; seed < tu::kSeeds; seed++) {
        tu::Gen g(seed);
        for (int n : tu::kSizes) {
            auto v = g.vec(n, -5, 5);
            ll best = LLONG_MIN;  // kadane: max non-empty subarray sum
            for (int l = 0; l < n; l++) {
                ll s = 0;
                for (int r = l; r < n; r++) s += v[r], best = max(best, s);
            }
            ASSERT_EQ(impl::kadane(v), best);

            auto c = impl::compress_array(v);
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) ASSERT_EQ(c[i] < c[j], v[i] < v[j]);
            ASSERT_EQ(*max_element(c.begin(), c.end()) + 1, (ll)set<ll>(v.begin(), v.end()).size());
            ASSERT_EQ(compressed_coords(v, v), c);

            ll mex = 0;
            while (count(v.begin(), v.end(), mex)) mex++;
            ASSERT_EQ(mex_of(v), mex) << "seed=" << seed << " n=" << n;

            auto [nge, nse] = nge_nse(n, v);
            for (int i = 0; i < n; i++) {
                ll a = n, b = n;
                for (int j = n - 1; j > i; j--) {
                    if (v[j] > v[i]) a = j;
                    if (v[j] < v[i]) b = j;
                }
                ASSERT_EQ(nge[i], a);
                ASSERT_EQ(nse[i], b);
            }

            int k = g.range(1, n);  // sliding window max / min of size k
            dq_max::monotone_deque<ll> mx;
            dq_min::monotone_deque<ll> mn;
            for (int i = 0; i < n; i++) {
                mx.insert(v[i]), mn.insert(v[i]);
                if (i >= k) mx.remove(v[i - k]), mn.remove(v[i - k]);
                if (i >= k - 1) {
                    ASSERT_EQ(mx.getmax(), *max_element(v.begin() + i - k + 1, v.begin() + i + 1));
                    ASSERT_EQ(mn.getmin(), *min_element(v.begin() + i - k + 1, v.begin() + i + 1));
                }
            }

            map<ll, ll> m1, m2, want;
            for (ll x : v) m1[x] += 1, m2[-x] += 2, want[x] += 1, want[-x] += 2;
            ASSERT_EQ(impl::merge_map(m1, m2), want);
        }
    }
    vector<pair<ll, ll>> p{{2, 1}, {1, 1}, {1, 5}, {2, 7}};
    sort(p.begin(), p.end(), impl::classcomp());  // first ascending, then second descending
    EXPECT_EQ(p, (vector<pair<ll, ll>>{{1, 5}, {1, 1}, {2, 7}, {2, 1}}));
}

TEST(Misc, HashesAndHilbertOrder) {
    gp_hash_table<ll, ll, impl::custom_hash> h;
    unordered_map<pair<ll, ll>, ll, impl::custom_hash> hp;
    map<ll, ll> ref;
    tu::Gen g(3);
    for (int i = 0; i < 20000; i++) {
        ll k = g.range(-1000, 1000);
        h[k] += i, ref[k] += i, hp[{k, -k}] += i;
    }
    for (auto [k, v] : ref) {
        ASSERT_EQ(h[k], v);
        ASSERT_EQ((hp[{k, -k}]), v);
    }

    set<ll> seen;  // distinct keys on a 64 x 64 grid
    for (int x = 0; x < 64; x++)
        for (int y = 0; y < 64; y++) ASSERT_TRUE(seen.insert(impl::hilbertorder(x, y)).second) << x << " " << y;
}

TEST(Misc, NamespaceIoRoundTrip) {
    io_in = tmpfile(), io_out = tmpfile();
    ASSERT_TRUE(io_in and io_out);
    IO::init();
    tu::Gen g(8);
    vector<ll> nums{0, -1, 9, 10, 99, 100, -100, LLONG_MAX, -LLONG_MAX};
    for (int i = 0; i < 100000; i++) nums.push_back(g.coin() ? g.range(-1000, 1000) : g.range(-INF, INF));
    string text;
    for (ll x : nums) {
        IO::write_int(x, ' ');
        text += to_string(x) + ' ';
    }
    IO::write_str("hello", '\n');
    IO::_flush_output();
    rewind(io_out);
    string got;
    for (int c; (c = fgetc(io_out)) != EOF;) got += (char)c;
    ASSERT_EQ(got, text + "hello\n");

    fputs((text + "  hello\n").c_str(), io_in);
    rewind(io_in);
    for (ll want : nums) {
        ll x;
        IO::read_int(x);
        ASSERT_EQ(x, want);
    }
    string s;
    IO::read_str(s);
    EXPECT_EQ(s, "hello");
}
